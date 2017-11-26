/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Nathan Osman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <fstream>

#include <boost/endian/conversion.hpp>

#include <win32pe/file.h>

#include "file_p.h"

using namespace win32pe;

const int DOSHeaderSize = 0x40;
const int PEOffsetOffset = 0x3c;

FilePrivate::FilePrivate()
{
}

FilePrivate::FilePrivate(const FilePrivate &other)
{
    *this = other;
}

FilePrivate::~FilePrivate()
{
}

FilePrivate &FilePrivate::operator=(const FilePrivate &other)
{
    mErrorString = other.mErrorString;
    mDOSHeader = other.mDOSHeader;
}

bool FilePrivate::readDOSHeader(std::istream &istream)
{
    // The DOS header is 64 bytes and contains the signature and offset to the
    // PE headers; keep all of the data in the DOS header up to the offset so
    // that it can be written back to the file later if necessary

    // Read the header
    mDOSHeader.resize(DOSHeaderSize);
    if (!istream.read(&mDOSHeader[0], DOSHeaderSize)) {
        mErrorString = "unable to read DOS header";
        return false;
    }

    // Confirm the signature
    if (mDOSHeader[0] != 'M' || mDOSHeader[1] != 'Z') {
        mErrorString = "file is missing MZ signature";
        return false;
    }

    // Determine the offset
    uint32_t peOffset = *reinterpret_cast<uint32_t*>(&mDOSHeader[0] + PEOffsetOffset);
    boost::endian::little_to_native_inplace(peOffset);

    // Read the rest of the data up to the PE headers
    mDOSHeader.resize(peOffset);
    if (!istream.read(&mDOSHeader[DOSHeaderSize], peOffset - DOSHeaderSize)) {
        mErrorString = "unable to read to PE headers";
        return false;
    }

    return true;
}

File::File()
    : d(new FilePrivate)
{
}

File::File(const File &other)
    : d(new FilePrivate(*other.d))
{
}

File::~File()
{
    delete d;
}

File &File::operator=(const File &other)
{
    *d = *other.d;
}

bool File::load(std::istream &istream)
{
    return d->readDOSHeader(istream);
}

bool File::load(const std::string &filename)
{
    std::ifstream ifstream(filename, std::ios::binary);
    if (ifstream.bad()) {
        d->mErrorString = "unable to open file";
        return false;
    }

    return load(ifstream);
}

std::string File::errorString() const
{
    return d->mErrorString;
}
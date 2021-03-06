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

#ifndef WIN32PE_FILE_P_H
#define WIN32PE_FILE_P_H

#include <istream>
#include <vector>

#include <win32pe/file.h>
#include <win32pe/fileheader.h>
#include <win32pe/importtable.h>
#include <win32pe/optionalheader.h>

namespace win32pe
{

class Section;

class FilePrivate
{
public:

    FilePrivate(File *file);

    FilePrivate &operator=(const FilePrivate &other);

    bool readDOSHeader(std::istream &istream);
    bool readPEHeaders(std::istream &istream);
    bool readSections(std::istream &istream);

    File *const q;

    std::string mErrorString;

    std::string mDOSHeader;
    FileHeader mFileHeader;
    OptionalHeader mOptionalHeader;

    std::vector<Section> mSections;

    ImportTable mImportTable;
};

}

#endif // WIN32PE_FILE_P_H

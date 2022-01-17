/*
 * Copyright (C) 2020, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Authors: zhangpengfei <zhangpengfei@kylinos.cn>
 * Modified by: zhangzihao <zhangzihao@kylinos.cn>
 * Modified by: zhangjiaping <zhangjiaping@kylinos.cn>
 *
 */
#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <QString>
#include <QCryptographicHash>
#include <QMap>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
//#include <QDomDocument>
#include <QQueue>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <uchardet/uchardet.h>

#define MAX_CONTENT_LENGTH 20480000

namespace Zeeker
{
class FileUtils
{
public:

    //chinese character to pinyin
    static QMap<QString, QStringList> map_chinese2pinyin;
    static QStringList findMultiToneWords(const QString &);
    static void loadHanziTable(const QString &);

private:
    FileUtils();
};
}

#endif // FILEUTILS_H

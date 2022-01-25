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
#include "file-utils.h"
#include <QXmlStreamReader>

QMap<QString, QStringList> FileUtils::map_chinese2pinyin = QMap<QString, QStringList>();

FileUtils::FileUtils()
{
}

void FileUtils::loadHanziTable(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug("File: '%s' open failed!", file.fileName().toStdString().c_str());
        return;
    }

    /* 读取汉字对照表文件并转换为QMap存储 */
    while (!file.atEnd()) {
        QString content = QString::fromUtf8(file.readLine());
        FileUtils::map_chinese2pinyin[content.split(" ").last().trimmed()] = content.split(" ").first().split(",");
    }

    file.close();
    return;
}

//DFS多音字太多直接GG
void stitchMultiToneWordsDFS(const QString &hanzi, const QString &resultAllPinYin, const QString &resultFirst, QStringList &resultList)
{
    if (hanzi.size() == 0) {
        resultList.append(resultAllPinYin);
        resultList.append(resultFirst);
        return;
    }

    if (FileUtils::map_chinese2pinyin.contains(hanzi.at(0))) {
        for (auto i : FileUtils::map_chinese2pinyin[hanzi.at(0)]) {
            stitchMultiToneWordsDFS(hanzi.right(hanzi.size() - 1), resultAllPinYin + i, resultFirst + i.at(0), resultList);
        }
    } else {
        stitchMultiToneWordsDFS(hanzi.right(hanzi.size() - 1), resultAllPinYin + hanzi.at(0), resultFirst + hanzi.at(0), resultList);
    }
}

//BFS+Stack多音字太多会爆栈
void stitchMultiToneWordsBFSStack(const QString &hanzi, QStringList &resultList)
{
    QString tempHanzi, resultAllPinYin, resultFirst;
    QQueue<QString> tempQueue;
    tempHanzi = hanzi;
    int tempQueueSize = 0;

    if (FileUtils::map_chinese2pinyin.contains(tempHanzi.at(0))) {
        for (auto i : FileUtils::map_chinese2pinyin[tempHanzi.at(0)]) {
            tempQueue.enqueue(i);
        }
    } else {
        tempQueue.enqueue(tempHanzi.at(0));
    }

    tempHanzi = tempHanzi.right(tempHanzi.size() - 1);

    while (tempHanzi.size() != 0) {
        tempQueueSize = tempQueue.size();

        if (FileUtils::map_chinese2pinyin.contains(tempHanzi.at(0))) {
            for (int j = 0; j < tempQueueSize; ++j) {
                for (auto i : FileUtils::map_chinese2pinyin[tempHanzi.at(0)]) {
                    tempQueue.enqueue(tempQueue.head() + i);
                }

                tempQueue.dequeue();
            }
        } else {
            for (int j = 0; j < tempQueueSize; ++j) {
                tempQueue.enqueue(tempQueue.head() + tempHanzi.at(0));
                tempQueue.dequeue();
            }
        }

        tempHanzi = tempHanzi.right(tempHanzi.size() - 1);
    }

    while (!tempQueue.empty()) {
        resultList.append(tempQueue.dequeue());
    }
}

//BFS+Heap，多音字太多会耗尽内存
void stitchMultiToneWordsBFSHeap(const QString &hanzi, QStringList &resultList)
{
    QString tempHanzi, resultAllPinYin, resultFirst;
    QQueue<QString> *tempQueue = new QQueue<QString>;
    tempHanzi = hanzi;
    int tempQueueSize = 0;

    if (FileUtils::map_chinese2pinyin.contains(tempHanzi.at(0))) {
        for (auto i : FileUtils::map_chinese2pinyin[tempHanzi.at(0)]) {
            tempQueue->enqueue(i);
        }
    } else {
        tempQueue->enqueue(tempHanzi.at(0));
    }

    tempHanzi = tempHanzi.right(tempHanzi.size() - 1);

    while (tempHanzi.size() != 0) {
        tempQueueSize = tempQueue->size();

        if (FileUtils::map_chinese2pinyin.contains(tempHanzi.at(0))) {
            for (int j = 0; j < tempQueueSize; ++j) {
                for (auto i : FileUtils::map_chinese2pinyin[tempHanzi.at(0)]) {
                    tempQueue->enqueue(tempQueue->head() + i);
                }

                tempQueue->dequeue();
            }
        } else {
            for (int j = 0; j < tempQueueSize; ++j) {
                tempQueue->enqueue(tempQueue->head() + tempHanzi.at(0));
                tempQueue->dequeue();
            }
        }

        tempHanzi = tempHanzi.right(tempHanzi.size() - 1);
    }

    while (!tempQueue->empty()) {
        resultList.append(tempQueue->dequeue());
    }

    delete tempQueue;
    tempQueue = nullptr;
}

//BFS+Heap+超过3个多音字只建一个索引，比较折中的方案
void stitchMultiToneWordsBFSHeapLess3(const QString &hanzi, QStringList &resultList)
{
    QString tempHanzi, resultAllPinYin, resultFirst;
    QQueue<QString> *tempQueue = new QQueue<QString>;
    QQueue<QString> *tempQueueFirst = new QQueue<QString>;
    tempHanzi = hanzi;
    int tempQueueSize = 0;
    int multiToneWordNum = 0;

    for (auto i : hanzi) {
        if (FileUtils::map_chinese2pinyin.contains(i)) {
            if (FileUtils::map_chinese2pinyin[i].size() > 1) {
                ++multiToneWordNum;
            }
        }
    }

    if (multiToneWordNum > 3) {
        QString oneResult, oneResultFirst;

        for (auto i : hanzi) {
            if (FileUtils::map_chinese2pinyin.contains(i)) {
                oneResult += FileUtils::map_chinese2pinyin[i].first();
                oneResultFirst += FileUtils::map_chinese2pinyin[i].first().at(0);
            } else {
                oneResult += i;
                oneResultFirst += i;
            }
        }

        resultList.append(oneResult);
        resultList.append(oneResultFirst);
        return;
    }

    if (FileUtils::map_chinese2pinyin.contains(tempHanzi.at(0))) {
        for (auto i : FileUtils::map_chinese2pinyin[tempHanzi.at(0)]) {
            tempQueue->enqueue(i);
            tempQueueFirst->enqueue(i.at(0));
        }
    } else {
        tempQueue->enqueue(tempHanzi.at(0));
        tempQueueFirst->enqueue(tempHanzi.at(0));
    }

    tempHanzi = tempHanzi.right(tempHanzi.size() - 1);

    while (tempHanzi.size() != 0) {
        tempQueueSize = tempQueue->size();

        if (FileUtils::map_chinese2pinyin.contains(tempHanzi.at(0))) {
            for (int j = 0; j < tempQueueSize; ++j) {
                for (auto i : FileUtils::map_chinese2pinyin[tempHanzi.at(0)]) {
                    tempQueue->enqueue(tempQueue->head() + i);
                    tempQueueFirst->enqueue(tempQueueFirst->head() + i.at(0));
                }

                tempQueue->dequeue();
                tempQueueFirst->dequeue();
            }
        } else {
            for (int j = 0; j < tempQueueSize; ++j) {
                tempQueue->enqueue(tempQueue->head() + tempHanzi.at(0));
                tempQueueFirst->enqueue(tempQueueFirst->head() + tempHanzi.at(0));
                tempQueue->dequeue();
                tempQueueFirst->dequeue();
            }
        }

        tempHanzi = tempHanzi.right(tempHanzi.size() - 1);
    }

    while (!tempQueue->empty()) {
        resultList.append(tempQueue->dequeue());
        resultList.append(tempQueueFirst->dequeue());
    }

    delete tempQueue;
    delete tempQueueFirst;
    tempQueue = nullptr;
    tempQueueFirst = nullptr;
    return;
}

//BFS+Stack+超过3个多音字只建一个索引，比较折中的方案
void stitchMultiToneWordsBFSStackLess3(const QString &hanzi, QStringList &resultList)
{
    QString tempHanzi, resultAllPinYin, resultFirst;
    QQueue<QString> tempQueue;
    QQueue<QString> tempQueueFirst;
    tempHanzi = hanzi;
    int tempQueueSize = 0;
    int multiToneWordNum = 0;

    for (auto i : hanzi) {
        if (FileUtils::map_chinese2pinyin.contains(i)) {
            if (FileUtils::map_chinese2pinyin[i].size() > 1) {
                ++multiToneWordNum;
            }
        }
    }

    if (multiToneWordNum > 3) {
        QString oneResult, oneResultFirst;

        for (auto i : hanzi) {
            if (FileUtils::map_chinese2pinyin.contains(i)) {
                oneResult += FileUtils::map_chinese2pinyin[i].first();
                oneResultFirst += FileUtils::map_chinese2pinyin[i].first().at(0);
            } else {
                oneResult += i;
                oneResultFirst += i;
            }
        }

        resultList.append(oneResult);
        resultList.append(oneResultFirst);
        return;
    }

    if (FileUtils::map_chinese2pinyin.contains(tempHanzi.at(0))) {
        for (auto i : FileUtils::map_chinese2pinyin[tempHanzi.at(0)]) {
            tempQueue.enqueue(i);
            tempQueueFirst.enqueue(i.at(0));
        }
    } else {
        tempQueue.enqueue(tempHanzi.at(0));
        tempQueueFirst.enqueue(tempHanzi.at(0));
    }

    tempHanzi = tempHanzi.right(tempHanzi.size() - 1);

    while (tempHanzi.size() != 0) {
        tempQueueSize = tempQueue.size();

        if (FileUtils::map_chinese2pinyin.contains(tempHanzi.at(0))) {
            for (int j = 0; j < tempQueueSize; ++j) {
                for (auto i : FileUtils::map_chinese2pinyin[tempHanzi.at(0)]) {
                    tempQueue.enqueue(tempQueue.head() + i);
                    tempQueueFirst.enqueue(tempQueueFirst.head() + i.at(0));
                }

                tempQueue.dequeue();
                tempQueueFirst.dequeue();
            }
        } else {
            for (int j = 0; j < tempQueueSize; ++j) {
                tempQueue.enqueue(tempQueue.head() + tempHanzi.at(0));
                tempQueueFirst.enqueue(tempQueueFirst.head() + tempHanzi.at(0));
                tempQueue.dequeue();
                tempQueueFirst.dequeue();
            }
        }

        tempHanzi = tempHanzi.right(tempHanzi.size() - 1);
    }

    while (!tempQueue.empty()) {
        resultList.append(tempQueue.dequeue());
        resultList.append(tempQueueFirst.dequeue());
    }

    //    delete tempQueue;
    //    delete tempQueueFirst;
    //    tempQueue = nullptr;
    //    tempQueueFirst = nullptr;
    return;
}

QStringList FileUtils::findMultiToneWords(const QString &hanzi)
{
    //    QStringList* output = new QStringList();
    QStringList output;
    QString tempAllPinYin, tempFirst;
    QStringList stringList = hanzi.split("");
    //    stitchMultiToneWordsDFS(hanzi, tempAllPinYin, tempFirst, output);
    stitchMultiToneWordsBFSStackLess3(hanzi, output);
    //    qDebug() << output;
    return output;
}

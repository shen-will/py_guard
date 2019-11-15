#!encoding=utf8


# 构建棋盘
import os

def buildLine(n):
    line = []
    for i in range(2 * n + 1):
        line.append('-')
    return line


def buildBlanks(n):
    blanks = []
    for i in range(n):
        blanks.append('|')
        blanks.append(' ')
    blanks.append('|')
    return blanks


def buildPlate(n):
    plate = []
    for i in range(n):
        plate.append(buildLine(n))
        plate.append(buildBlanks(n))
    plate.append(buildLine(n))
    return plate


def show(plate):
    for row in plate:
        print(''.join(row))


# 落子
def dropPiece(plate, row, col, piece):
    row = 2 * row - 1
    col = col * 2 - 1
    plate[row][col] = piece


# 判断胜负
def judge(plate):
    # 判断是否有落子的空位，如果没有，则返回
    # 判断每一行字符都相同，有则返回该字符，从而判断胜负
    n = len(plate)
    for i in range(1, n - 1, 2):
        isEqua = True
        first = plate[i][1]
        for j in range(1, n - 1, 2):
            if plate[i][j] != first:
                isEqua = False
                break
        if isEqua:
            return first
    # 判断每一列的字符都相同，有则返回该字符
    for i in range(1, n - 1, 2):
        isEqua = True
        first = plate[1][i]
        for j in range(1, n - 1, 2):
            if plate[j][i] != first:
                isEqua = False
                break
        if isEqua:
            return first
    # 判断对角线的字符都相同，有则返回该字符
    isEqua = True
    for i in range(1, n - 1, 2):
        first = plate[1][1]
        if plate[i][i] != first:
            isEqua = False
            break
    if isEqua:
        return first
    x = n - 2
    y = 1
    z = y
    first = plate[x][y]
    while True:
        x -= 2
        y += 2
        if first != plate[x][y]:
            isEqua = True
            break
        if x == z:
            break
    if not isEqua:
        return first
    # first=plate[5][1]
    # if plate[5][1]==plate[3][3]==plate[1][5]:
    #     return first


def main():
    i = 1
    a = int(input("输入棋盘大小:"))
    plate = buildPlate(a)
    show(plate)
    m = len(plate)
    while True:
        piece = 'o'
        if i % 2 == 0:
            piece = 'x'
        try:
            x = int(input("enter the location x:"))
            y = int(input("enter the location y:"))
        except Exception as ex:
            print("请输入0-" + str(a) + "以内的数")
            continue

        if not (0 <= x <= a and 0 <= y <= a):
            print("请输入0-" + str(a) + "以内的数")
            continue
        if plate[2 * x - 1][2 * y - 1] == ' ':
            dropPiece(plate, x, y, piece)
        else:
            print("此位置已有棋子")
            continue
        i += 1
        show(plate)
        if judge(plate) == 'o' or judge(plate) == 'x':
            print("the winner is " + judge(plate))
            break
        s = 0
        for j in range(1, m - 1, 2):
            for k in range(1, m - 1, 2):
                if plate[j][k] == ' ':
                    s = s + 1
        if s == 0:
            print("平局")
            break



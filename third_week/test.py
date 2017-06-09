
while True:
    print '> ',
    line = raw_input()
    answer = 0
    index = 0
    number = 0
    while index < len(line):
        if line[index].isdigit():
            while index < len(line) and line[index].isdigit():
                number = number * 10 + int(line[index])
                index += 1
            if index < len(line) and line[index] == '.' :
                index += 1
                divide = 0.1
                while index < len(line) and line[index].isdigit():
                    number = number * divide+ int(line[index])
                    index += 1
                    divide *= 0.1

        elif line[index] == '+':
            index += 1
            answer += number
        elif line[index] == '-':
            index += 1
            answer -= number
        else:
            answer=number

        print number
        number = 0

    print "answer = %d\n" % answer

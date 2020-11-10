import re

#Opening the file
file1 = open('../root/engine/engine.cpp','r')
Lines = file1.readlines()
lineCount = 0

#REGEX's
pattPlus = ' \+ '
pattSub = ' \- '
pattStar = ' \* '
pattCout = ' << '
pattCin = ' >> '
pattMod = ' % '
pattGreat = ' > '
pattLess = ' < '
pattAssign = ' = '
pattEqual = ' == ' #Need to come up with way to check for multiple occurences

#HELPER method to count number of symbols in a line
def countSymbol(string, symbol):
    count = 0
    doubleSymbol = 0
    #This outer if statement returns the valid number of single 'symbol's differentiating betwen >, <, +, and -
    if (len(symbol) == 1):
        if(symbol == '+'):
            match = re.findall('\+\+',string)
            if (match): 
                doubleSymbol = countMatch(match) * 2
            for i in string:
                if i == symbol:
                    count = count + 1
            count = count - doubleSymbol
        elif(symbol == '<'):
            match = re.findall('<<',string)
            if (match): 
                doubleSymbol = countMatch(match) * 2
            for i in string:
                if i == symbol:
                    count = count + 1
            count = count - doubleSymbol
        elif(symbol == '>'):
            match = re.findall('>>',string)
            if (match): 
                doubleSymbol = countMatch(match) * 2
            for i in string:
                if i == symbol:
                    count = count + 1
            count = count - doubleSymbol
        elif(symbol == '-'):
            match = re.findall('--',string)
            if (match): 
                doubleSymbol = countMatch(match) * 2
            for i in string:
                if i == symbol:
                    count = count + 1
            count = count - doubleSymbol
        elif(symbol == '='):
            match = re.findall('==',string)
            if (match): 
                doubleSymbol = countMatch(match) * 2
            for i in string:
                if i == symbol:
                    count = count + 1
            count = count - doubleSymbol
    #checks for > 1 length symbols like ++, ==, --,         
    else:
        regex = symbol
        match = re.findall(symbol,string)
        count = countMatch(match)
    return count

#HELPER method to count number of matches
def countMatch(match):
    count = 0
    for ma in match:
        count = count + 1
    return count

#HELPER method to check for operators takes in Operator, LineCount, RegexPattern, Line
def checkOperator(oper,count,pattern,line): 
    if (line.find(oper) > - 1) :
        match = re.finditer(pattern,line) #takes in regex pattern, and given line
        numOfSymbol = countSymbol(line,oper) #takes in given line, and operator SYMBOL so '+' or '<'
        numOfMatch = countMatch(match) #counts number regex patterns found in line
        if(match and numOfSymbol == numOfMatch) : 
            random = 0
        else :
            print("Bad spacing on line " + str(count) + " with the operator " + oper)

#Makes sure that keywords have correct styling
def checkKeyword(count,line,lines):
    if (line.find("while") > - 1):
        match = re.search(r'\s*while (.*) {[\s]*$',line)
        if(match):
            random = 0
        else:
            print("Spacing error on WHILE loop OR missing a bracket at end of while(...) { on line " + str(count)) 
    if (re.search(r'\s*for (.*)\s*',line)):
        if (re.search('\s*',lines[count + 2])):
            random = 0
        else:
            print("For loops with one line SHOULD not have braces")
    elif (line.find("for") > - 1):
        match = re.search(r'\s*for (.*) {[\s]*$', line)
        if(match):
            random = 0
        else:
            print("Spacing error on FOR loop OR missing bracket at end of for(...) { on line " + str(count))

#Loop that checkstyles each line
for line in Lines:
    lineCount = lineCount + 1
    if(line.find("#") > -1) :
        line = line[line.find("#")] #parses comments 
    if(line.find("//") > -1) : 
        line = line[0:line.find("//")] #parses comments
    if(line.find("/*") > -1):
        random = 0
    if(re.search("^\s\*",line)):
        random = 0
    else:
        #checks Keywords like For While and one line Loops
        checkKeyword(lineCount,line,Lines)
        #checks binary operators
        checkOperator('+',lineCount,pattPlus,line)
        checkOperator('-',lineCount,pattSub,line)
        checkOperator('*',lineCount,pattStar,line)
        checkOperator('%',lineCount,pattMod,line)
        checkOperator('=',lineCount,pattAssign,line)
        checkOperator('<<',lineCount,pattCout,line)
        checkOperator('>>',lineCount,pattCin,line)
        checkOperator('==',lineCount,pattEqual,line)
        #TODO >, < 
        checkOperator('>',lineCount,pattGreat,line)
        checkOperator('<',lineCount,pattLess,line)
    





 







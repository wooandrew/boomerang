import re

#Opening the file
file1 = open('main.cpp','r')
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
    if(len(symbol) == 1):
        if(symbol == '<'):
            later = "do this later"
        elif(symbol == '>'):
            later = "do this later too"
        for i in string:
            if i == symbol:
                count = count + 1
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
        match = re.finditer(pattern,line)
        numOfSymbol = countSymbol(line,oper)
        numOfMatch = countMatch(match)
        if(match and numOfSymbol == numOfMatch) : 
            random = 0
        else :
            print("Bad spacing on line " + str(count) + " with the operator " + oper)

for line in Lines:
    lineCount = lineCount + 1
    if(line.find("#") > -1) :
        line = line[line.find("#")] #parses comments 
    if(line.find("//") > -1) : 
        line = line[0:line.find("//")] #parses comments
    
    checkOperator('+',lineCount,pattPlus,line)
    checkOperator('-',lineCount,pattSub,line)
    checkOperator('*',lineCount,pattStar,line)
    checkOperator('%',lineCount,pattMod,line)
    #checkOperator('>',lineCount,pattGreat,line)
    #checkOperator('<',lineCount,pattLess,line) implement later :)
    checkOperator('=',lineCount,pattAssign,line)
    
   #NEED TO FIND WAY TO COUNT STRINGS OF LENGTH > 1
    checkOperator('<<',lineCount,pattCout,line)
    checkOperator('>>',lineCount,pattCin,line)
    checkOperator('==',lineCount,pattEqual,line)






 







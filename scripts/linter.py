import re

#Opening the file
file1 = open('test.cpp','r')
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

def checkKeyword(count,line,Lines):
    if (line.find("while") > - 1):
        match = re.search(r'\s*while (.*) {[\s]*$',line)
        if(match):
            random = 0
        else:
            print("Spacing error on WHILE loop OR missing a bracket at end of while(...) { on line " + str(count)) 
    if (line.find("for") > - 1):
        match = re.search(r'\s*for (.*) {[\s]*$', line)
        if(match):
            random = 0
        else:
            print("Spacing error on FOR loop OR missing bracket at end of for(...) { on line " + str(count))

for line in Lines:
    lineCount = lineCount + 1
    if(line.find("#") > -1) :
        line = line[line.find("#")] #parses comments 
    if(line.find("//") > -1) : 
        line = line[0:line.find("//")] #parses comments
    
    checkKeyword(lineCount,line,Lines)
    checkOperator('+',lineCount,pattPlus,line)
    checkOperator('-',lineCount,pattSub,line)
    checkOperator('*',lineCount,pattStar,line)
    checkOperator('%',lineCount,pattMod,line)
    checkOperator('=',lineCount,pattAssign,line)
    
   #NEED TO FIND WAY TO COUNT STRINGS OF LENGTH > 1
    checkOperator('<<',lineCount,pattCout,line)
    checkOperator('>>',lineCount,pattCin,line)
    checkOperator('==',lineCount,pattEqual,line)
    #TODO >, < AND ++, -- OPERATORS






 







# Contributing to Project Boomerang (v1)
:point_right: Howdy! Thank you for your interest in Project Boomerang. This guideline will discuss how you can appropriately contribute to the project. While this guideline is mostly meant for outside contributers, collaborators should also heed general guidelines. Keep in mind this is a guideline, not a set of rules. You don't necessarily need to stick to every word!

### Table of Contents
[Code of Conduct](#code-of-conduct)<br>
[Got Questions?](#got-questions?)<br>
[Contacting a Collaborator](#contacting-a-collaborator)<br>

[What Should I know?](#what-should-i-know)<br>
- [Basics](#basics)
- [Dependencies](#dependencies)
- [Licensing](#licensing)
- [Credits](#credits)

[Style Guide](#style-guide)<br>
- [Git Commit Messages](#git-commit-messages)
- [C++ Style](#c++-style)

[Other Guidelines](#other-guidelines)


## Got Questions?
Got a question about contributing or about Project Boomerang in general? Go ahead and join our [Discord Server](https://discord.gg/NaM4FtJ) and ask one of the collaborators.

## Code of Conduct
Before contributing, please read the project [Code of Conduct](CODE_OF_CONDUCT.md). This project is thusly governed, and all contributors and collaborators are expected to withhold these guidelines. Please report any violation of these guidelines to seungminleader@gmail.com, using the guidelines discussed in the next section.

## Contacting a Collaborator
Want to directly contact a collaborator? You can email us, following these guidelines:

Your email subject should include the following:<br>
`[Project Boomerang] - OneWordProblem - Longer Description`<br>
ex. `[Project Boomerang] - Question - How do I contact a collaborator?`

You should sign your email as such: <br>
```
FirstName LastName or Alias
your email address
optional: phone # 
```

`seungminleader@gmail.com` :arrow_right: Andrew Woo (Project Manager, Developer)<br>
`pyertersquires@gmail.com` :arrow_right: Porter Squires (Developer)<br>
`awrishkhan@gmail.com` :arrow_right: Awrish Khan (Developer)

## What Should I Know?

### Basics
Project Boomerang is written in C++17 using the OpenGL framework, and is entirely open source. For now, it is meant to be cross-platform between Windows and Linux, as Apple has deprecated the OpenGL standard. However, if we ever decide to port the project to Vulkan, we will try our best to support MacOS as well. Project Boomerang utilizes CMake as its build tool.

### Dependencies
For now, Project Boomerang's dependencies will include:
```
- GLFW              version 3.3
- GLAD              version 4.6 core
- GLM               version 0.9.9
- stb_image.h       version 2.26
- PortAudio         version 19
- FreeType          version 2.4
```
Dependencies may change as project progresses. None of the dependencies will be provided in the repository due to licensing issues. You will need to download and build the dependencies yourself.

### Licensing
Project Boomerang in and of itself is licensed under a permissive Modified MIT License. Any code written by contributers will thusly be licensed under this same license, and the contributer will be appropriately credited for the code.
```
Modified MIT License

Copyright 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * Restrictions:
 >  The Software may not be sold unless significant, mechanics changing modifications are made by the seller, or unless the buyer
 >  understands an unmodified version of the Software is available elsewhere free of charge, and agrees to buy the Software given
 >  this knowledge.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ```

 ### Credits
 We give credit where credit is due. If you contribute code that we use, you will be appropriately credited, even if the code is unrecognizably modified after implementation, but not during implementation. Contributors will be credited as a contributor, not as a collaborator. In the unlikely case that we decide to sell the software, either in its entirety or copies of, collaboraters will be given equal share of the profit, but contributors will not be paid at all.

 ## Style Guide
 As this is a multi-person project, we expect differing coding styles. As such, this is an attempt to unify style within the repository.

 ### Git Commit Messages
 The commit message itself should be date followed by time in the following format: <br>
 `MM/DD/YYYY - HH:MM AM/PM :: Short Description` <br>
 ex. `11/02/2020 - 01:38 PM :: Fixed bug #0001`

 and the dscription may be anything that describes the change in more detail.

 ### C++ Style
Here are the basic styling guides for C++. A linter will be designed and provided in the future. All functions should be written in C++ style, not C style, unless C style is absolutely necessary. In other words, `std::cout` should be preferred over `printf()`.<br>

Code blocks should be indicated by 4-space indentations, and curly braces should open at the same line as function declarations, not afterwards:

```cpp
// --- Good ------------------
int main() {

    bool run = true;

    while(run) {

        std::cout << "Hello, Project Boomerang!" << std::endl;
        run = false;
    }

    return 0;
}

// --- Bad -------------------
int main() 
{

bool run = true;

   while(run) 
   {
    printf("printf is C Style, not C++ Style");
    run = false;
   }

     return 0;
}
```
For if/else statements and loops, curly braces should only be utilized for multiple line blocks. Further, any block of two or more lines should have a newline above:
```cpp
// --- Good ------------------
if(contidional == true) {

    for(auto x : aList)
        std::cout << x << std::endl;

    if(conditional2 == true)
        conditional = false;
}

// --- Bad -------------------
if(contidional == true) {
    for(auto x : aList) {
        std::cout << x << std::endl;
    }

    if(conditional2 == true){
        conditional = false;
    }
}
```
Similar objects should be grouped together. Further, all variables should be initialized to a value to prevent undefined behavior:
```cpp
// --- Good ------------------
int x = 0;
int y = 0;

std::string output = "";

Object object1 = Object(x, y, output);
Object object2 = Object();

// --- Bad -------------------
int x = 0; int y = 0;

std::string output = "";

Object object1 = Object(x, y, output);

Object object2 = Object();

```
More guidelines will be added later.

## Other Guidelines
This repository will use a standard master-develop git workflow. That is, all code will be commited to the develop branch, and merged to the master branch when appropriate. Please ensure when you are creating a pull request that the pull request is to the develop branch. Pull requests to the master branch will be ignored.

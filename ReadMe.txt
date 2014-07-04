Description: this is the client of the C/S architecture.

Rule to commit:
    0. Pull the code from the repository on BitBucket and store in some place you want.
    1. Write the code you want to commit.
    2. Add the files modified by using "$ git add".
    3. Commit the code by using "$ git commit", the command line will show you a text editor.
       Write some infomation about what have you done in this commit, the format is:
       first line: What have you done in this commit, e.g. "Add a feature of ***". (should be just ONE sentence)
       second line:just press entry.
       third line: The detail what you did. It is a description of what is in your code.
       third line: the branch name of this commit. e.g. "GVSClient-105".
    4. Close the text editor and push the code upon BitBucket by using "$ git push origin GVSClient-105".

To compile and link, you should add a env-path on your system:
    Name: VTKDIR
    Value: the path vtk installed on the system.
  If the path has not been set correctly, vs2010 could not find the header files and libraries.
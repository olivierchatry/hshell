# TODO

 - echo "/bin/ls /hbtn ; echo \$?" | ./hsh --> 0 (should be 2)
 -not found variable should be replaced by empty string

### shell_getcwd.c

When in a symbolic link to a directory, bash stores the name of the link instead of its path

Example: link named "test" in "/home/user", pointing to "/tmp"

If we are in this folder, bash will set its PWD to "/home/user/test"

In the current version of this program, the PWD is set to "/tmp"

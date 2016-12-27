#!/bin/bash

command="alias b=echo
alias a=b
a Hello Guillaume"

echo "$command" | ./simple_shell > test

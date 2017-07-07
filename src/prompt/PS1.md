# Implemented

- `\d`: The date in "Weekday Month Date" format (e.g., "Tue May 26")
- `\H`: The hostname
- `\h`: the hostname up to the first `.`
- `\s`: The name of the shell, the basename of $0 (the portion following the final slash)
- `\u`: The username of the current user
- `\v`: the version of bash (e.g., 2.00)
- `\W`: the basename of the current working directory, with $HOME
abbreviated with a tilde
- `\w`: The current working directory, with $HOME abbreviated with a tilde (uses the value of the PROMPT_DIRTRIM variable)
- `\$`: if the effective UID is 0, a #, otherwise a $

# Not implemented

- `\a`: an ASCII bell character (07)
- `\D{format}`
the format is passed to strftime(3) and the result is
inserted into the prompt string; an empty format results
in a locale-specific time representation. The braces are
required
- `\e`: an ASCII escape character (033)
- `\j`: the number of jobs currently managed by the shell
- `\l`: the basename of the shell's terminal device name
- `\n`: newline
- `\r`: carriage return
- `\t`: the current time in 24-hour HH:MM:SS format
- `\T`: the current time in 12-hour HH:MM:SS format
- `\@`: the current time in 12-hour am/pm format
- `\A`: the current time in 24-hour HH:MM format
- `\V`: the release of bash, version + patch level (e.g., 2.00.0)
- `\!`: the history number of this command
- `\#`: the command number of this command
- `\nnn`: the character corresponding to the octal number nnn
- `\\`: a backslash
- `\[`: begin a sequence of non-printing characters, which could
be used to embed a terminal control sequence into the
prompt
- `\]`: end a sequence of non-printing characters

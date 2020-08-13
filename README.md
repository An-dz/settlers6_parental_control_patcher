# Why

Windows 7 and above contain a feature called Game Explorer, this feature causes
some older games to fail or display weird behaviour and so many users disable
this feature by removing _gameux.dll_ from they System32 and SysWOW64 folders.

This dll is also responsible for managing parental controls for games and
unfortunately **Settlers 6** was coded in a way that it considers parental
controls to be enabled if this dll can't be loaded.

This patcher changes that by disabling the feature completely so users that have
disabled Game Explorer don't have to re-enable it just for this game.

# Using

Just copy the executable available on the [releases page][1] next to the game
executable and run. A backup of the original executable is created.

The executable can be found on `The Settlers 6/base/bin` for the base game and
`The Settlers 6/extra1/bin` for the expansion pack. The executables are exactly
the same.

If you get a permission denied error it's probably because the game is installed
on Program Files and so the patcher needs to be run as admin to change the file.

This patcher was only tested on the GOG version of the game (which is the
recommended version as it already contains other compatibilty patches). It
should work on any other version but it's not guaranteed. You can contact me if
it fails on the Steam or retail versions.

# How

This patcher changes a single bit in the executable so the variable that holds
the parental control check has a value that passes the tests.

Here's the relevant assembly and binary as viewed on [x64dbg](http://x64dbg.com):

```diff
 0040BA89 | FF15 D8748F00            | call dword ptr ds:[<&GetSystemMetrics>] |
 0040BA8F | 8BF0                     | mov esi, eax                            |
 0040BA91 | E8 83FEFFFF              | call settlers6.40B919                   |
-0040BA96 | 8BF8                     | mov edi, eax                            |
+0040BA96 | 8BF9                     | mov edi, ecx                            |
 0040BA98 | E8 E5750600              | call settlers6.473082                   |
 0040BA9D | 84C0                     | test al, al                             |
 0040BA9F | 74 04                    | je settlers6.40BAA5                     |
```

As you may see the binary was changed from `0x8DF8` to `0x8DF9`, or as assembly
from `mov edi, eax` to `mov edi, ecx`. Here **eax** contains the result from the
parental control check and **edi** is the variable that will hold for the
subsequent tests. Thus changing to get the data from **ecx** is enough to trick
the code into accepting parental controls are not enabled.


[1]: https://github.com/An-dz/settlers6_parental_control_patch/releases

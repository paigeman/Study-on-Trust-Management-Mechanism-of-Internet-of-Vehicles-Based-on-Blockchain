# How to use

The header files in the `include` directory include the header files in the src directory of NS3 project. Although there must be a way that make the header files in the `include` directory copy the content of the header files in the src directory of NS3 project. However, I do not know how to make it. Therefore, you have to put the source code of NS3 project in the same position as me or replace the paths of the header files in the `include` directory to your paths globally, e.g., replace the content of `#include "replace here"`. The latter is easy to achieve it with the help of text editor.

It is recommended to add the path of lib directory to the `PATH` environment variable, or you can add it after encountering certain errors.

Or you can just ignore these things, and follow the steps in issue #14.

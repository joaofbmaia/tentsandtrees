# tents-and-trees-validator

## C style and coding rules
- Do not use tabs, **use spaces** instead
- Use **4 spaces** per indent level
- Use **1 space** between keyword and opening bracket
- Do not use space between function name and opening bracket
- Use **lowerCamelCase** for variables/functions/macros/types
- Opening curly **bracket** is always at the **same line** as keyword (`for`, `while`, `do`, `switch`, `if`, ...)
- Use **single space** before and after comparison and assignment operators
- Use **single space** after every comma
- **Avoid vertical whitespace** (when needed, no more than one empty line)

## Clang-Format style
- BasedOnStyle: Google
- IndentWidth: 4
- ColumnLimit: 0
- SpaceAfterCStyleCast: true
- DerivePointerAlignment: false
- PointerAlignment: Right
- If using VS Code change *C_Cpp.clang_format_fallbackStyle* setting to `{BasedOnStyle: Google, IndentWidth: 4, ColumnLimit: 0, SpaceAfterCStyleCast: true, DerivePointerAlignment: false, PointerAlignment: Right}`

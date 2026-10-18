---
title: Add a menu option
description: Add a row to the options screen of the menu.
---

# Add a menu option

!!! note
    The names below are the ones of the current menu. If a name changed, search for it in `Menu.cpp`: the idea stays the same.

The menu is made of screens. A screen has a number of rows, a title and a help line, all written in one table in `Menu`. To add a row to the options:

1. **Store the value.** Add a field to `Settings` with a default value.
2. **Count the row.** Increase the number of rows of the options screen in the table.
3. **Draw it.** In the function that draws the options, add a call to `DrawRow` with its label and its current value.
4. **Handle it.** In the function that handles the options, react to left, right and Enter on the new row.
5. **Use it.** Read the value where it matters (the volume goes to the audio, the text size goes to the layout).

Keep the same keys as the other screens: up and down to move, left and right to change a value, Enter to select, Esc to go back. This is part of the [accessibility](../../project/accessibility.md) of the menu.

!!! warning "If you remove a row"
    Change the number of rows too. A row removed from the drawing but still counted leaves an invisible row that can be selected.

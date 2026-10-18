# How-to guides

Each guide is a list of steps. Do them in order, then build.

## Add a source file

1. Create the `.cpp` and `.hpp` files, with a PascalCase name.
2. Add the `.cpp` to the `CMakeLists.txt` of the folder.
3. Run `cmake --build --preset default`.

## Add a sound effect

1. Put the sound file in `Assets/Sounds` (PascalCase name).
2. In `Audio.hpp`, add the name to `enum class Effect`.
3. Add the file to `EffectFiles`, **in the same order** as the enum.
4. Call `Play(Effect::YourEffect)` where the sound must happen.

## Add a stage music

1. Put the file in `Assets/Sounds`.
2. Add it to `StageMusics`.

A stage after the last one keeps the last music.

## Add a texture

1. Put the image in `Assets`, in the matching folder (for example `Assets/Title/RTypeTitle.png`). Use a PascalCase name.
2. Load it with `ResourceManager::GetTexture("Title/RTypeTitle.png")`, after `InitWindow`.

The path is relative to the `Assets` folder and is case sensitive.

## Add a menu option

The options screen has one row for each option, then the **Back** row. To add an option (for example `Brightness`):

1. In `Settings.hpp`, add the setting with its default value.
2. In `Menu.hpp`, increase `kOptionsCount` by one and declare `void AdjustBrightness(const Input &input);`.
3. In `Menu.cpp`, write `AdjustBrightness`: read `input.step` (left or right) and `input.activate` (Enter or click), and change the setting.
4. In `HandleOptions`, add a `case` for the new row that calls `AdjustBrightness`. The **Back** row moves down by one: change the index of its `case` and of its `GoTo`.
5. In `DrawOptions`, add a `DrawOptionRow` for the new row, and move the **Back** row down.
6. Keep each function under 25 lines.

## Add a field to the connect form

1. In `ConnectForm.hpp`, add the field, a length constant, and a static `IsXxxChar` function that says which characters are allowed.
2. Check the field in `ConnectForm::Validate` (cut the check in its own function if `Validate` gets long).
3. In `Menu`, increase `kFieldCount` and `kConnectCount`, add a `case` in `SelectedField`, and add a `DrawFieldRow` in `DrawConnect`.

## Add a document page

1. Create a `.md` file in `Docs`.
2. Add it to `nav` in `mkdocs.yml`.
3. Check it with `mkdocs serve`.
4. Open a pull request: the site is published when the change reaches `main`.

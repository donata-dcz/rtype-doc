---
title: Add an image
description: Add a texture to the client and display it.
---

# Add an image

1. Put the image in `Assets`, in a folder if it helps (`Assets/Title/RTypeTitle.png`). The name is in PascalCase: the CI refuses underscores.
2. Load it with the `ResourceManager`, **after** `InitWindow`:

   ```cpp
   const Texture2D &logo = resources.GetTexture("Title/RTypeTitle.png");
   ```

   The first call loads the image and the next calls give the same texture.

3. Draw it with Raylib (`DrawTexturePro` or `DrawTextureEx`). Compute the size from the size of the window, so it still looks right when the window is resized.

!!! tip
    If the image does not appear, check the name first: the path is spelled the same as the file, with the same capital letters, and the file was copied next to the executable.

# Agents of Mayhem patch to fix crash with Steam Input
Works with the latest version of the game on Steam (v1.06).

## Installation
1. Download the file `VERSION.dll` from the [latest release](https://github.com/JnCrMx/aom-sd-controller-fix/releases/latest).
2. Move this file into the `aom` subfolder inside the game folder (`.../steamapps/common/Agents of Mayhem/aom`).
3. Set the Launch Options of the game to:
   ```
   WINEDLLOVERRIDES="version=n,b" %command%
   ```
5. Enable Steam Input for the game.
6. Select the "Official Layout of Agents of Mayhem - Official Configuration" by "Deep Silver Volition" as the Controller Layout
   (if you didn't change it, this layout should already be selected).
7. Launch the game as normal.

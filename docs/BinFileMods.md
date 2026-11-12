# Bin File Mods

If your mod needs to replace a bin file, the BaseMod can handle that without overwritting the original bin file. Just create a `Resource` folder in your mod folder and place your bin file(s) in that folder with the same folder structure. Your mod does not need a dll component to replace bin files. You can just make a mod folder that only has a Resource sub folder (a config.json is not required either).

Example: If you were to replace the `side_belt.bin` file your folder structure should be:

```text
Guilty Gear XX Accent Core Plus R/
├── Resource
|   └── demo
|       └── side_belt.bin (original bin)
└── mods/
    └── MyExampleMod/
        └── Resource
            └── demo
                └── side_belt.bin (your modded bin)
```

If one bin file is modded multiple times by different mods, only the one in the last mod folder alphabetically takes affect.

## Description
This module provides a system for customizing mount requirements in AzerothCore.  
It allows you to safely:

* Modify Riding Skill training costs and level requirements (including Tome of Cold Weather Flight)
* Modify Buy/Sell prices and level requirements for mounts.  
* Roll back to default values easily
* Separate configuration for Paladin, Warlock, Druid, and Death Knight class mounts available

## Requirements
A recent version of AzerothCore is recommended. This module uses hard-coded item and spell IDs  
that it expects to find within the `acore_world.item_template` and `acore_world.npc_trainer` tables.

## Installation
1. Download mod-mount-requirements into the `modules` directory of your AzerothCore source. 
2. Copy the `mod_mount_requirements.conf.dist` file from the `conf` folder into your AzerothCore source's module config folder. (`../path/to/azerothcore-wotlk/env/dist/etc/modules`). **Rename the file** to `mod_mount_requirements.conf` (remove the `.dist`).
3. Re-run cmake and launch a clean build of AzerothCore. On linux you can run `../path/to/azerothcore-wotlk/acore.sh compiler build` . (Check official AzerothCore documentation for more info: https://www.azerothcore.org/wiki/installing-a-module)

## Usage
**Note: Everyone on your server will need to delete their game's `Cache` folder between updates to the .conf file.** Not doing so will result in players seeing old tooltip data, and they may be unable to "learn" new mounts even if they meet your configured requirements. This is because the game client relies on cached information for some of it's internal checks.  

1. Stop your server if it's already running.  
2. Configure the `mod_mount_requirements.conf` file as needed.  
3. Make sure Wow.exe is not running, then, delete the `Cache` folder in the same directory as Wow.exe (this gets recreated every time the game runs).  
4. Start the server, and log in.  

### To revert changes made by the module  
1. Follow the same steps as above, but set `MountRequirements.Enable = false` in your `mod_mount_requirements.conf` file first.  
2. The next time the server runs, all the original mount requirements will be applied to the database. (*Remember to delete your `Cache` folder*)
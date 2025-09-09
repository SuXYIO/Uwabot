# Uwabot

## Intro

Well, short for _Underwater Bot_, stores code for school project.  
For the underwater team of _Affiliated High School of Peking University (BDFZ)_.

<del>There's no official English name for the team as far as I know, so I just made this name up.</del>

## Intro

Well, short for _Underwater Bot_, stores code for school project.  
For the underwater team of _Affiliated High School of Peking University (BDFZ)_.

## File Structure

| File                              | Functionality                                                      |
| --------------------------------- | ------------------------------------------------------------------ |
| `Uwabot.ino`                      | The main file for arduino, acts like `main()` function             |
| `pins.h`                          | Define macro of pin numbers, to avoid _Magic Numbers_ in code      |
| `configLora.cpp` & `configLora.h` | Contains the functions for configuring _Lora_ communication module |

The concept is having pairs of `foo.cpp` and `foo.h`, providing functions to call and a _header_ to `#include`.

## Roadmap

- Bot motion control
  - [ ] Implement motion control for bot
- `Lora` module **\***
  - [x] Implement init for `Lora`
  - [ ] Implement communication test for `Lora`
  - [ ] Implement `Lora` remote controller
- Automation **\***
  - [ ] Implement automatic mode for bot

Note: **\*** means to implement according to the competition rules.  
Some competition requires remote, while others requires auto.  
Might have to make different versions.

## References

Some code in here might reference (and even adopt) results and code in the [underwater team documents](https://pkuschool.yuque.com/vkrd4i/iybowe), outsiders probably could not read stuff in there though.

And [here](http://www.ashining.com.cn/data/a39c_t400a22d1a.zip) is the link to docs for `Lora` module the team bought.

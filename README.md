# Alternity STD

**STL & libm like HEADER-ONLY library** with a lot of extras written specifically for Alternity Arts projects.
Initial purpose of this library was to learn how to implement stl-like library in practice, while it still stands its now more like generic library used for any project of mine

#### Disclaimer: Is still WIP and for ease of use there tends to be typedefs from std:: and library utilizes std in some places. Is intended to be replaced by custom implementation

| **Supported Platforms** | **Tested** | **Macro** |
|:-|:-:|:-:|
| Windows | YES | PLATFORM_WINDOWS |
| Linux | YES | PLATFORM_LINUX |
| Apple | YES | PLATFORM_APPLE |

| **Supported Architectures** | **Tested** | **Macro** |
|:-|:-:|:-:|
| 64 | YES | ARCHITECTURE_64 |
| 32 | NO | ARCHITECTURE_32 |

| **Supported Compilers** | **Tested** | **Macro** |
|:-|:-:|:-:|
| MSVC | YES | COMPILER_MSVC |
| GNUC_CLANG | YES | COMPILER_GNUC && COMPILAR_GNUC_CLANG |
| GNUC_INTEL | NO | COMPILER_GNUC && COMPILER_GNUC_INTEL |
| GNUC_GCC | YES | COMPILER_GNUC && COMPILER_GNUC_GCC |

## Features:

| **Description**                     | **Example** |
|:------------------------------------|:-:|
| STL-like containers 	               | TArray, TSharedPtr 								|
| Lightweight Mathematic library			   | SMath::Abs, SMath::Lerp 							|
| Asynchronous callback object	 			   | TAsync				 							|
| Human readable build macros 				    | BUILD_RELEASE/PLATFORM_WINDOWS/ARCHITECTURE_32	|
| A lot of handy and extra features 		| TSharedClass, TAsync 								|
| Object oriented helper functions	 		| SString::Split		 							|
| Character encoding with tchar 			   | tchar* = TEXT("myText") 							|
| Debug breaks in code			 			         | if (ensure(condition)) 							|

## STL-like features:

| **Description** | **ASTD** | **STD equivalent** |
|:-|:-:|:-:|
| Smart Pointers							| TSharedPtr, TWeakPtr 				| shared_ptr, weak_ptr 	|
| Dynamic containers						| TArray ... 						| vector ...			|
| Dynamic string							| SString  							| string				|
| High precision time manipulation			| STimer  							| chrono				|

### Dynamic Containers

| **Description** | **ASTD** | **STD equivalent** |
|:-|:-:|:-:|
| Sequence container						| TArray 							| vector			|
| FIFO container 							| TQueue 							| queue				|
| Optional value container					| TOptional 						| optional			|

## PCH
Library supports two types of like-PCH files.
### ASTDMinimal.h
Holds minimal includes, most of which are macros, templates, forward declarations and core platform code
### ASTD.h
Holds all features, declarations and implementation of all types within STD

## Platform code
To make it easier for writing bigger chunks of platform code. Each platform has its corresponding folders which can be included via **PLATFORM_HEADER** macro.
For example **ASTD/Linux/LinuxMemory.h** and **ASTD/Memory.h**, which leverage **PLATFORM_HEADER** to get platform header from folder located in the root.
### How to write platform code
Make use of macros like **PLATFORM_HEADER**, **PLATFORM_STRUCT** or **PLATFORM_CLASS**. These are declared by platform and resolved by **ASTD/Build.h** include.
```
#include "ASTD/ASTDMinimal.h" // or "ASTD/Build.h"
#include PLATFORM_HEADER(MyHeader)

```

## Q&A
### But it's in global namespace ! :(
Library is not intended to be encapsulated in its own namespace. While it is not intended, it doesn't mean its impossible to achieve.
You can just use provided PCH that should include all necessary headers and encapsulate include itself:
```
namespace ASTD
{
    #include "ASTD/ASTD.h"
}

int main()
{
    ASTD::SString myStr = TEXT("Hello World!");
    std::cout << myStr << std::endl;
    return EXIT_SUCCESS;
}
```

# CPPrelude

**CPPrelude** is a body of code that I tend to write over and over again in every project I make. So, I decided to write it as a library and include it in my projects.

It contains algorithms, containers, memory management stuff ... etc.

Developed by:

**[Moustapha Saad Abdel-hameed](mailto:moustapha.saad.abdelhamed@gmail.com)**

**[Nora Abdel-hameed](mailto:nora.abdelhameed@gmail.com)**

**CPPrelude** version 1.0.0 is released under the GNU Lesser General Public License version 3.

## Philosophy and Coding standard

For Philosophy and coding standard please refer to this [link](https://moustaphasaad.github.io/2017/08/05/cpprelude/).

[How CPPrelude does polymorphism?](https://moustaphasaad.github.io/2018/01/06/How-CPPrelude-does-polymorphism/)

## Building

1. **CPPrelude** uses premake5 as its meta build system so you have to download premake5
2. Use premake5 to generate project/solution files in order to compile the library.
   1. Ex: `premake5 gmake` this will generate the build folder with linux make files inside
   2. Ex: `premake5 vs2015` this will generate the build folder with visual studio 2015 solution files inside
3. **CPPrelude** supports 32-bit and 64-bit targets and has a *debugShared* and *releaseShared* modes
4. building the library is as easy as invoking your build system
5. binaries should be generated inside a bin folder right beside the build folder
6. Also a unittest executable will be generated you can invoke it to test your **CPPrelude** build

## Content

- **[Algorithms](cpprelude/Algorithms.md):** Documentation of the included algorithms
- **[Allocator_Trait](cpprelude/Allocator_Trait.md):** Documentation of the Allocator_Trait type.
- **[Allocators](cpprelude/Allocators.md):** Documentation of the available allocators.
- **[Array](cpprelude/Array.md):** Documentation of the static Array type. 
- **[Benchmark](cpprelude/Benchmark.md):** Documentation of the micro benchmark library.
- **[defines](cpprelude/defines.md):** Documentation of the available basic types and some macros.
- **[Double_List](cpprelude/Double_List.md):** Documentation of the Double_List type.
- **[Dynamic_Array](cpprelude/Dynamic_Array.md):** Documentation of the Dynamic_Array type.
- **[File](cpprelude/File.md):** Documentation of the File type.
- **[File_Handle](cpprelude/File_Handle.md):** Documentation of the File_Handle type.
- **[Hash](cpprelude/Hash.md):** Documentation of the Hash functions.
- **[Hash_Map](cpprelude/Hash_Map.md):** Documentation of the Hash_Map and Hash_Set type.
- **[IO](cpprelude/IO.md):** Documentation of the IO functions.
- **[IO_Trait](cpprelude/IO_Trait.md):** Documentation of the IO_Trait type.
- **[Memory_Context](cpprelude/Memory_Context.md):** Documentation of the Memory_Context type.
- **[Memory_Stream](cpprelude/Memory_Stream.md):** Documentation of the Memory_Stream type.
- **[OS](cpprelude/OS.md):** Documentation of the OS type.
- **[Owner](cpprelude/Owner.md):** Documentation of the Owner type.
- **[Panic](cpprelude/Panic.md):** Documentation of the panic functions.
- **[Priority_Queue](cpprelude/Priority_Queue.md):** Documentation of the Priority_Queue type.
- **[Queue_List](cpprelude/Queue_List.md):** Documentation of the Queue_List type.
- **[Ranges](cpprelude/Ranges.md):** Documentation of the Ranges types.
- **[Result](cpprelude/Result.md):** Documentation of the Result type.
- **[Ring_Array](cpprelude/Ring_Array.md):** Documentation of the Ring_Array type.
- **[Single_List](cpprelude/Single_List.md):** Documentation of the Single_List type.
- **[Stack_Array](cpprelude/Stack_Array.md):** Documentation of the Stack_Array type.
- **[Stack_List](cpprelude/Stack_List.md):** Documentation of the Stack_List type.
- **[String](cpprelude/String.md):** Documentation of the String type.
- **[Tree_Map](cpprelude/Tree_Map.md):** Documentation of the Tree_Map type.

## How to contribute

Feel free to fork and create pull requests at any time. It would be nice if you involved any one of us with you from the start to guide you, or maybe discuss your idea itself (Developers e-mails are up above).

## Thanks
- [Ahmed Hesham](mailto:ahesham@outlook.com) for adding Visual Studio debug visualizer.
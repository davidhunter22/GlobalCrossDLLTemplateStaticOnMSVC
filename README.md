# Work around for a Microsoft  Compiler/Linker issue

Given a template like the following

    template <typename T> struct Foo 
    {     
        static inline T value; 
    }

you would expect that if you instantiate the template for a given type T say int that there would only be one Foo::value in your program.
However if you are using Microsoft Visual Studio, this has been tested up to at least 16.8, and you use the Foo<int> in mutliple
DLLs you will get one instance of Foo::value per DLL.

This is surprising to many people and there are many cases of people reporting it and attempting to get it to work in the manner 
that most people expect, that there is only on "value" for a given type T across a whole program. See below for examples of people posting about this.

As far as I can tell Microsoft's view is that the standard doesn't speak about what happens across DLL boundaries and that this is just the 
way the compiler/linker work. Which is fine, I don't want to get into that.

There are a number of possible work around suggested for this in various posts these fall primarily into two camps

### Use explicit template instantiation
In your Foo header you can create an explicit template instantiation for a given type, so
    template Foo<int>;  // Explicit instantiation of Foo for an int
You may then need to mess around with __declspec(dllexport) and __declspec(dllimport) but the idea is to get other DLLs
to use this instantiation. It would be nice to have an example of this work around but this is not it.
The big problem in many cases is that the writer of Foo does not know in most cases what types Foo is going to be instantiated with.
Even worse even if they did they would need to include the header file for each of those type into Foo.h. This means a possibly
low level type like Foo would be using higher level types. This will almost certainly cause mutual recursion in both header include
and DLL references. Maybe there is a way to use an approach like this

### Use a type lookup map with a type erased value representing the global instance
In various posts you see this approach using say

    std::map<std::type_index,void*>

So the type erasure is done by casting a pointer to the memory of your global instance to a void pointer.
This project is a variant of this but using

    std::unordered_map<std::type,std::any>

Using std::any gives us better type safety and make the code simpler.
The constraints for the solution are
1. The Foo header should not know what types it is being instantiated
2. The code in other DLL that uses Foo\<T> should have to do no special code, they simply can use Foo\<T> as normal
3. A lower priority, the solution should be easy to #ifdef in and out so the simple obvious code above can be used easily for tool chains that work as expecte.d

In the example code the Foo type is actual called Counter.


## Posts concerning this topis, please add more

1. https://www.gamedev.net/forums/topic/544029-c-singleton-across-dlls/
2. https://stackoverflow.com/questions/10520587/how-to-implement-a-singleton-in-an-application-with-dll
3. https://developercommunity.visualstudio.com/content/problem/743697/is-this-an-odr-violation-with-static-inline-variab.html
4. https://stackoverflow.com/questions/1007585/getting-loki-singleton-to-work-in-dlls-in-vs-2008-c
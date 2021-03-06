# Work around for a Microsoft  Compiler/Linker issue

Given a template like the following

    template <typename T> struct Counter 
    {
        T get_value( ) ...

        void set_value( T t ) ...
       
    private:

        static inline T value; 
    }

you would expect that if you instantiate the template for a given type T say int that there would only be one Counter::value in your program.
However if you are using Microsoft Visual Studio, this has been tested up to at least 16.8, and you use the Counter<int> in mutliple
DLLs you will get one instance of Counter::value per DLL.

This is surprising to many people and there are many cases of people reporting it and attempting to get it to work in the manner 
that most people expect, that there is only on "value" for a given type T across a whole program. See below for examples of people posting about this.

As far as I can tell Microsoft's view is that the standard doesn't speak about what happens across DLL boundaries and that this is just the 
way the compiler/linker work. Which is fine, I don't want to get into that.

There are a number of possible work around suggested for this in various posts these fall primarily into two camps

### Use explicit template instantiation
In your Counter header you can create an explicit template instantiation for a given type, so
    template Counter<int>;  // Explicit instantiation of Counter for an int
You may then need to mess around with __declspec(dllexport) and __declspec(dllimport) but the idea is to get other DLLs
to use this instantiation. It would be nice to have an example of this work around but this is not it.
The big problem in many cases is that the writer of Counter does not know in most cases what types Counter is going to be instantiated with.
Even worse even if they did they would need to include the header file for each of those type into Counter.h. This means a possibly
low level type like Counter would be using higher level types. This will almost certainly cause mutual recursion in both header include
and DLL references. Maybe there is a way to use an approach like this

### Use a type lookup map with a type erased value representing the global instance
In various posts you see this approach using say

    std::map<std::type_index,void*>

So the type erasure is done by casting a pointer to the memory of your global instance to a void pointer.
This project is a variant of this but using

    std::unordered_map<std::type,std::any>

Using std::any gives us better type safety and make the code simpler.
The constraints for the solution are
1. The Counter header should not know what types it is being instantiated
2. The code in other DLL that uses Counter\<T> should have to do no special code, they simply can use Counter\<T> as normal
3. A lower priority, the solution should be easy to #ifdef in and out so the simple obvious code above can be used easily for tool chains that work as expecte.d

## Comments on the code

#### How to set the initial value of the global instance
One question that arises if how do you set an initial value for the global instance, meaning both the memory it consumes and it's initial value.
The work around at present automatically create a default constructed instance of the contained type on first use. This seems reasonable as
it's what happens if your on a tool chain that doesn't need this work around. Well almost the same as in that case the memory is presumably allocated
at program start up rather than on first use. 

#### Constraints on type T
The work around may impose more constraints on T then a tool chain not using it. It would be good if the constraints were shown to be the same
to avoid surprising users.

#### Performance
Obviously this is slower as every use is doing a look up based on the type_index of T. It is using an unorderd_map so at least this is a hash based lookup
but this is probably not an appropriate work around if this lookup becomes a dominant part of run time.

#### Usage
This work around has been used in a fairly large C++ code base without problems.

## Posts concerning this topis, please add more

1. https://www.gamedev.net/forums/topic/544029-c-singleton-across-dlls/
2. https://stackoverflow.com/questions/10520587/how-to-implement-a-singleton-in-an-application-with-dll
3. https://developercommunity.visualstudio.com/content/problem/743697/is-this-an-odr-violation-with-static-inline-variab.html
4. https://stackoverflow.com/questions/1007585/getting-loki-singleton-to-work-in-dlls-in-vs-2008-c
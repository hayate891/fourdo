#include <iostream.h>

#include "types.h"
#include "Console.h"

void main()
{
   Console* con;

   con = new Console ();
   delete con;
}
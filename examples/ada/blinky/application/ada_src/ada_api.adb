package body Ada_API is

   function Add_N_To_M (N, M : int) return int is
      Add : Integer := 0;
   begin

      for I in Integer(N) .. Integer(M) loop
         Add := Add + I;
      end loop;
      
      return int(Add);
      
   end Add_N_To_M;

end Ada_API;


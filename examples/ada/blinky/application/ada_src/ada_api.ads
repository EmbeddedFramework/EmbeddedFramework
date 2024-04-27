with Interfaces.C; use Interfaces.C;

package Ada_API is

   function Add_N_To_M (N, M : int) return int
     with
       Export        => True,
       Convention    => C,
       External_Name => "Add_N_To_M";

end Ada_API;


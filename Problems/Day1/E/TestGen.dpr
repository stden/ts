{$apptype console}
uses
   SysUtils;

function foo(k, l: integer): string;
var
   r: integer;
begin
  Result := '';
  while k <> 0 do begin
     r := k mod l;
     k := k div l;
     if r > 9 then
        Result := chr(ord('A') + r - 10) + Result
     else
        Result := chr(ord('0') + r) + Result;
  end;
end;

procedure Gen(n: integer);
var
   k, l: integer;
begin
   k := random(1 shl ((n div 2) + 1)) + 1;
   l := random(15) + 2;
   assign(output, inttostr(n)); rewrite(output);
   write(foo(k, l), ' ', l);
   close(output);
   assign(output, inttostr(n) + '.a'); rewrite(output);
   write(k);
   close(output);   
end;

var
   i: integer;
begin
   Randomize;
   for i := 1 to 30 do Gen(i);
end.

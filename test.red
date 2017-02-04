ls -la >toto
cat toto ; rm toto
ls -la >toto|cat -n|cat <titi
cat toto ; rm toto
ls -la >toto>tata
cat toto; echo "\n"; cat tata ; rm toto tata
ls -la | cat -n |cat -n |cat -n >toto>tata
cat toto; echo "\n"; cat tata ; rm toto tata
echo "trololo pouet toto" | cat -n |cat -n |cat -n >toto
cat toto ; rm toto
ls -la >toto; ls -1
cat toto ; rm toto
ls -la | cat -n |cat -n >toto>tata ; echo "lolilol"; cat <toto ; echo "lolilol"; cat <tata
ls -la | cat <noexist|cat -n
echo "trololo pouet toto" >toto; ls; ls -la |cat <toto; rm toto
echo "trololo pouet toto" >toto; ls; ls -la |cat <toto |cat -n; rm toto

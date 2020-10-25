all:prod cons dph

prod:
      gcc proc.c -o prod -lpthread -lrt -lm
      
cons:
      gcc cons.c -o cons -lpthread -lrt -lm
      
dph:
      gcc dph.c -o dph -lpthread

NAME = liveserver

SRC  = server/server.cpp server/HttpServer.cpp server/methods.cpp server/utils/utils.cpp
OBJ  = $(SRC:.cpp=.o)
CC   = g++
CFLAGS = -std=c++17 -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
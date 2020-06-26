/******************************************************************************

MAZE SOLVER
INF1010 Estrutura de Dados Avançadas
Prof. Augusto Baffa

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fila_vet.h"
#include "pilha_vet.h"
#include "maze.h"

#define DELAY 5

#define SHOW_PROCESSING 1
#define ALG 0   // 0 DFS, 1 PILHA_DFS, 2 BFS 
	
int dfs(int v, PlayerDef* player, MazeDef* maze, int visited[maze_getGraphV(maze)]){

	int state = 0;
	
	int w;
	visited[v] = 1;

	player->current_vertex = v;
	player->current_y = vertex_to_map_y(player->current_vertex, maze_getFileCols(maze));
	player->current_x = vertex_to_map_x(player->current_vertex, maze_getFileCols(maze));
	player->steps++;
	 
	if(SHOW_PROCESSING) {
		display(player, maze);
		printf(" >> Executando DFS\n");
		
		msleep(DELAY);
	}
	
	if(maze_getFileTile(maze, player->current_y, player->current_x) == 'F')
		return 1;

	for (w = 0; w < maze_getGraphV(maze); w++){
		if (maze_getGraphEdge(maze, v,w) && !visited[w]){
			state = dfs(w, player, maze, visited);
			if(state)
				break;
		}
	}
	
	return state;
}


int pilha_dfs(Pilha* pilha, PlayerDef* player, MazeDef* maze, int visited[maze_getGraphV(maze)]){

	if(pilha_vazia(pilha))
		return 0;

	int state = 0;
	int v = pilha_pop(pilha);
	int w;
	visited[v] = 1;

	player->current_vertex = v;
	player->current_y = vertex_to_map_y(player->current_vertex, maze_getFileCols(maze));
	player->current_x = vertex_to_map_x(player->current_vertex, maze_getFileCols(maze));
	player->steps++;
	 
	if(SHOW_PROCESSING) {
		display(player, maze);
		printf(" >> Executando PILHA_DFS\n");
		
		msleep(DELAY);
	}

	if(maze_getFileTile(maze, player->current_y, player->current_x) == 'F')
		return 1;

	for (w = 0; w < maze_getGraphV(maze); w++){
		if (maze_getGraphEdge(maze, v,w) && !visited[w]){
			pilha_push(pilha, w);
		}
	}

	state = pilha_dfs(pilha, player, maze, visited);
	
	return state;
}


int bfs(Fila* fila,  PlayerDef* player, MazeDef* maze, int visited[maze_getGraphV(maze)]){

	if(fila_vazia(fila))
		return 0;

	int state = 0;
	int v = fila_retira(fila);
	int w;
	visited[v] = 1;

	player->current_vertex = v;
	player->current_y = vertex_to_map_y(player->current_vertex, maze_getFileCols(maze));
	player->current_x = vertex_to_map_x(player->current_vertex, maze_getFileCols(maze));
	player->steps++;
	
	if(SHOW_PROCESSING) {
		display(player, maze);
		printf(" >> Executando BFS\n");
		
		msleep(DELAY);
	}

	if(maze_getFileTile(maze, player->current_y, player->current_x) == 'F')
		return 1;

	for (w = 0; w < maze_getGraphV(maze); w++){
		if (maze_getGraphEdge(maze, v,w) && !visited[w]){
			fila_insere(fila, w);
		}
	}

	state = bfs(fila, player, maze, visited);
	
	return state;
}


int main() {
	clock_t t;
	int v,w;
	
	MazeDef* maze = maze_criaLabirinto();
	
	maze = maze_carregaLabirinto(maze);

	maze = maze_carregaMatrizAdjacencias(maze);
	
	//print_matrix(maze, 2000, 2020);

	PlayerDef* player = maze_criaPlayer();

	player->current_x = maze_getStartX(maze);
	player->current_y = maze_getStartY(maze);

	player->current_vertex = map_to_vertex(player->current_y, player->current_x, maze_getFileCols(maze));

	display(player, maze);
	
	int visited[maze_getGraphV(maze)];
	
	for (w = 0; w < maze_getGraphV(maze); w++) // inicializa visitados    
		visited[w] = 0;

	if(ALG == 0){

		///////////////////////////////////////////////////////
		// chamar DFS
		
		t = clock(); 
		if(dfs(player->current_vertex, player, maze, visited))
		{
			t = clock() - t; 
			display(player, maze);
			printf("Final encontrado c/ DFS em %f segundos\n", ((double)t)/CLOCKS_PER_SEC);
			
			for(w = 0; w < maze_getGraphV(maze); w++)
				if(visited[w])
					maze_print_path(maze, w);
		}
		else{
			t = clock() - t; 
			display(player, maze);
			printf("Não encontrado c/ DFS em %f segundos\n", ((double)t)/CLOCKS_PER_SEC);
		}

	}
	else if(ALG == 1){

		///////////////////////////////////////////////////////
		// chamar PILHA_DFS
		Pilha* pilha = pilha_cria ();
		pilha_push(pilha, player->current_vertex);
		
		t = clock(); 
		if(pilha_dfs(pilha, player, maze, visited)){
			t = clock() - t; 
			display(player, maze);
			printf("Final encontrado c/ DFS em %f segundos\n", ((double)t)/CLOCKS_PER_SEC);
			
			for(w = 0; w < maze_getGraphV(maze); w++)
				if(visited[w])
					maze_print_path(maze, w);
		}
		else{
			t = clock() - t; 
			display(player, maze);
			printf("Não encontrado c/ DFS em %f segundos\n", ((double)t)/CLOCKS_PER_SEC);
		}
	}
	else if(ALG == 2){

		///////////////////////////////////////////////////////
		// chamar BFS
		Fila* fila = fila_cria ();
		fila_insere(fila, player->current_vertex);
		
		t = clock(); 
		if(bfs(fila, player, maze, visited)){
			t = clock() - t; 
			display(player, maze);
			printf("Final encontrado c/ BFS em %f segundos\n", ((double)t)/CLOCKS_PER_SEC);
	
			for(w = 0; w < maze_getGraphV(maze); w++)
				if(visited[w])
					maze_print_path(maze, w);        
		}
		else{
			t = clock() - t; 
			display(player, maze);
			printf("Não encontrado c/ BFS em %f segundos\n", ((double)t)/CLOCKS_PER_SEC);
		}
	}

	printf ("\033[H");
	printf("\033[%dB", maze_getFileRows(maze)+6);
	
	return 0;
}
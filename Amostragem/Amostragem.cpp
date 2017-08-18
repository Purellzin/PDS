#include <math.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#define PI 3.1416
#include <stdlib.h>

using namespace std;

string converter(int n, int q_casas){
	string r;
    while(n != 0) {
		r = (n%2 == 0 ? "0":"1") + r;
	 	n = n/2;
	}
	while(r.length() < q_casas){
		r = "0" + r;
	}
    return r;
}

int main() {
	stringstream stream1, stream2;
	int d = 0, nIntervalos = 0, b = 0;
	char l;
	float amplitude = 0 , frequencia = 0, intervaloQuantizador = 0;
	float temporario = 0;

	do{
		cout<<"Digite a amplitude:";
		cin>>amplitude;
		system("cls");
		cout<<"Digite a frequencia:";
		cin>>frequencia;
		system("cls");
		cout<<"Digite o valor de (b)?";
		cin>>b;


		nIntervalos = pow(2,b);

		/*Definicao dos vetores*/
		string intervalo_q[nIntervalos];
		float xq[nIntervalos], xeq[nIntervalos], amostragem[nIntervalos];
		float nivel_e_coddecimal[nIntervalos][2], intervalos[nIntervalos][2];

		/*Calculo da amostragem*/
		cout<<" 1 - seno(t) \t 2 - cosseno(t)";
		cout<<"Digite o tipo de funcao que deseja calcular:";
		cin>>d;
		ofstream func("funcao.txt");
		func<<"#coluna 1 = t\n\n";
		func<<"#coluna 2 = x[t]\n";
		switch(d){

			case 1:
				cout<<"Funcao seno(t)";
				for(int t = 0; t < nIntervalos; t++){
					amostragem[t] = roundf((amplitude*(sin(2*PI*frequencia*t))*10000))/10000;
				}
			    for(float a=0; a < nIntervalos; a = a + 0.01){
	    			func<<a<<"\t"<<amplitude*sin(2*PI*frequencia*a)<<"\n";
				}
			break;
			case 2:
				cout<<"Funcao cosseno(t)";
				for(int t = 0; t < nIntervalos; t++){
					amostragem[t] = roundf((amplitude*(cos(2*PI*frequencia*t))*10000))/10000;
				}
				for(float a=0; a < nIntervalos; a = a + 0.01){
	    			func<<a<<"\t"<<amplitude*cos(2*PI*frequencia*a)<<"\n";
				}
			break;
		}

		func.close();

		/*Intervalo do quantizador*/
		intervaloQuantizador = (amplitude - (-amplitude))/nIntervalos;

		temporario = amplitude;
		for(int i = (nIntervalos-1); i >= 0; i--){
			/*Calculo dos intervalos*/
			stream1 << fixed << setprecision(2) << temporario;
			stream2 << fixed << setprecision(2) << (temporario-intervaloQuantizador);
			intervalo_q[i] = stream1.str() + " a " + stream2.str();

			intervalos[i][0] = round(temporario*100)/100; //Intervalo inicial
			intervalos[i][1] = round((temporario-intervaloQuantizador)*100)/100; //Intervalo final

			//Limpa a string
			stream1.str("");
			stream2.str("");

			/*Nivel de quantizacao*/
			for(int a=0; a<2; a++){
				nivel_e_coddecimal[i][1] = (temporario + (temporario-intervaloQuantizador))/2;
			}
			nivel_e_coddecimal[i][0] = i;

			temporario = (temporario-intervaloQuantizador);

		}

		/*Calculos da segunda tabela*/
		for(int t = 0; t < nIntervalos; t++){
			for(int i = (nIntervalos - 1); i >= 0; i--){
				if(nivel_e_coddecimal[i-1][1] < amostragem[t] and amostragem[t] < nivel_e_coddecimal[i][1]){
					xq[t] = nivel_e_coddecimal[i][1];
				}
			}
			/*Erro de quantizacao*/
			xeq[t] = round((amostragem[t] - (xq[t]))*1000)/1000;
		}

		/*Primeira Tabela*/
		cout<<"  Intervalo q\t Nivel de q\tCodificacao";
		for(int i = (nIntervalos - 1); i >= 0; i--){
			cout << setfill(' ') << setw(15) << intervalo_q[i];
			cout << setfill(' ') << setw(10) << nivel_e_coddecimal[i][1];
			cout << setfill(' ') << setw(10) << nivel_e_coddecimal[i][0];
			cout << setfill(' ') << setw(5) << converter((int)nivel_e_coddecimal[i][0],b);
			cout<<"\n";
	 }

		/*Segunda Tabela*/
		cout<<" Amostra x[n] \t xq[n]\t xeq[n]\tdecimal  binario";
		for(int i = 0; i < nIntervalos; i++){
			cout << setfill(' ') << setw(5) << i;
			cout << setfill(' ') << setw(10) << amostragem[i];
			cout << setfill(' ') << setw(8) << xq[i];
			cout << setfill(' ') << setw(8) << xeq[i];
			for(int t = 0; t < nIntervalos; t++){
				if(nivel_e_coddecimal[t][1] == xq[i]){
					cout << setfill(' ') << setw(5) << nivel_e_coddecimal[t][0];
					cout << setfill(' ') << setw(9) << converter((int)nivel_e_coddecimal[t][0],b);
					t = nIntervalos;
				}
			}
			cout<<"\n";
		}

		cout<<"Digite 's' para efetuar um novo calculo:";
		cin>>l;
	}while(l=='s' || l=='S');
	return 0;
}

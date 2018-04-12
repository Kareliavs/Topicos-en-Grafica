#include <bits/stdc++.h>
#include <dirent.h>
#include "PORTER/OleanderStemmingLibrary-master/stemming/english_stem.h"
/*#include "stemming/french_stem.h"
#include "stemming/german_stem.h"
#include "stemming/finnish_stem.h"
#include "stemming/swedish_stem.h"
#include "stemming/spanish_stem.h"
#include "stemming/dutch_stem.h"
#include "stemming/danish_stem.h"
#include "stemming/italian_stem.h"
#include "stemming/norwegian_stem.h"
#include "stemming/portuguese_stem.h"
#include "stemming/russian_stem.h"*/
/*
wget -O boost_1_55_0.tar.gz http://sourceforge.net/projects/boost/files/boost/1.55.0/boost_1_55_0.tar.gz/download
tar xzvf boost_1_55_0.tar.gz
cd boost_1_55_0/
*/
///sudo apt-get install libboost-dev
#include <boost/algorithm/string.hpp>    
#include <regex>
using namespace std;
///compila asi ---> g++ filtro.cpp -o a
ofstream bow("BOW.txt"); 
vector<double> top;
vector< pair < double , string> >TopBOW;
map <string, int>  BagOfWords;
map <string, int>  NumDocPal;
map <string,  map <string, double> >  matriz;
vector<vector<double> > vect;

map< string , int >::iterator iter,iter2;
stemming::english_stem<> StemEnglish;

vector<string> stopwords;
string ruta="AP_BBC_CNN_Reuters_nosource_nodate_novo_nofull";//"CORPUSPRUEBA";
struct Documento {
        string nombre;
        int numPal;
        map <string, int> BOW_propio;
};
vector<Documento> Corpus;

int ActualizarBOW(string palabra)
{   BagOfWords[palabra]++;	
}
void LeerStopWords()
{
	ifstream fichero;
    fichero.open(("stopwords.txt"));
    string frase,token;
    if (fichero.fail()) {
        cout << "Error al abrir el archivo stopwords.txt"<< endl;
    } 
    else
    {
    	while (!fichero.eof()) {
    		getline(fichero,frase);
    		stopwords.push_back(frase);
    	}
    }
}
bool EsStopWord(string palabra)
{
	for(int i=0;i<stopwords.size();i++)
	{   //cout<<palabra<<" vs "<<stopwords[i]<<endl;
		if(palabra==stopwords[i])
		{
			return true;
		}	
	}
	return false;
}
map <string, int>  LeerArchivo(string doc) {
	//doc = "./"+ruta+"/"+doc;
    ifstream fichero; //variable fichero para leer los datos
    fichero.open(("./"+ruta+"/"+doc));
    string frase,token;
    map <string, int>  BagOfWords_Doc;
    int num=0;
    if (fichero.fail()) {// si fichero no falla coge un letra dle fichero
        cout << "Error al abrir el archivo "<< doc << endl;

    } 
    else {
    	//cout<<"Entre5"<<endl;
    	int salta_linea=0;
        int buscar_cierre=0;
        int cadena=0;
        string cadenita;

        while (!fichero.eof()) {//mientras fichero no llegue al final
        	//listarecorrido.push_back(caracter);

            //fichero.get(caracter);
            getline(fichero,frase);
            if(frase.size()!=0)
            {	//cout<<endl<<"la frase: "<<frase<<endl;
	            ///Tokens
	            istringstream iss(frase);
				while(getline(iss,token,' '))
				{
				    //cout<<token<<", ";
				    regex word("[a-zA-Z]+?[a-zA-Z]*");
				    if(regex_match(token,word))
				    {
				    	boost::algorithm::to_lower(token);
				    	//cout<<token<<" "<<EsStopWord(token)<<endl;
				    	if(!EsStopWord(token))
				    	{
				    		//cout<<"ENTRE";
				    		std::wstring word(L"");
						    std::wstring wsTmp(token.begin(), token.end());
	    					word = wsTmp;
						    StemEnglish(word);
						    //cout<<" "<<token<<"\t ";
						    //std::wcout << L"(English) Stemmed text:\t"<<" --> "<< word.c_str() << std::endl;
					    	string s( word.begin(), word.end() );
					    	num++;
					   		ActualizarBOW(s);
					    	BagOfWords_Doc[s]++;
					    	//cout<<" "<<token<<", ";
				    	}

					}
				}
			}

        }
    }
    Documento d;
    d.nombre=doc;
    d.numPal=num;
    d.BOW_propio=BagOfWords_Doc;
    Corpus.push_back(d);
    for(iter=BagOfWords_Doc.begin(); iter!=BagOfWords_Doc.end();iter++)
		NumDocPal[iter->first]++;
	/*for(iter=NumDocPal.begin(); iter!=NumDocPal.end();iter++)
		cout<<iter->first<<"->"<<NumDocPal[iter->first]<<endl;*/
    fichero.close();
    return BagOfWords_Doc;
}
void LlenarMatriz()
{
	double TF;
	double IDF;
	for(int i=0;i<Corpus.size();i++)
	{
		for(iter=Corpus[i].BOW_propio.begin(); iter!=Corpus[i].BOW_propio.end();iter++)
		{	
				//frec_de_la_palabra_en_el_doc / NumPal_del_documento
			TF=(double)iter->second/(double)Corpus[i].numPal;
			////log(#documentos / #documentos_que_contienen_palabra )		
			IDF=log((double)Corpus.size()/(double)NumDocPal[iter->first]);
			matriz[Corpus[i].nombre][iter->first]=(double)TF*(double)IDF;
			//cout<<iter->first<<endl
			//	<<"TF : "<<TF<<" = "<<iter->second<<" / "<<Corpus[i].numPal<<" = "<<(double)iter->second/(double)Corpus[i].numPal<<endl
			//	<<"IDF: "<<IDF<<" = "<<"log( "<<(double)Corpus.size()<<" / "<< (double)NumDocPal[iter->first]<<" = "<<log((double)Corpus.size()/(double)NumDocPal[iter->first])<<endl;
			
		}
		vector<double> vect1;
		for(iter=BagOfWords.begin(); iter!=BagOfWords.end();iter++)
		{//cout<<"THIS: "<<matriz[Corpus[i].nombre][iter->first]<<endl;
			if(matriz[Corpus[i].nombre][iter->first]!=0)
			{	//cout<<"PONGO "<<Corpus[i].nombre<<" , "<<iter->first<<" = "<<matriz[Corpus[i].nombre][iter->first]<<endl;
				vect1.push_back(matriz[Corpus[i].nombre][iter->first]);
			}
			else vect1.push_back(0);
		}
		vect.push_back(vect1);
		
	}

	//////
	cout<<endl<<endl<<"\t";
	bow<<"\t";
	for(iter=BagOfWords.begin(); iter!=BagOfWords.end();iter++)
		{	cout<<iter->first<<"\t";
			bow<<iter->first<<"\t";
			top.push_back(0);
		}
	cout<<endl;
	bow<<endl;
	for(int i=0;i<vect.size();i++)
	{	cout<<Corpus[i].nombre<<"\t";
		bow<<Corpus[i].nombre<<"\t";
		for(int j=0;j<vect[i].size();j++)
		{
			//cout<<vect[i][j]<<"\t";
			top[j]+=vect[i][j],
			bow<<vect[i][j]<<"\t";
		}
		cout<<endl;
		bow<<endl;
	}
	int i=0;
	for(iter=BagOfWords.begin(); iter!=BagOfWords.end();iter++)
	{
		TopBOW.push_back(make_pair(top[i],iter->first));
		i++;
	}
	sort(TopBOW.begin(),TopBOW.end());
	/*for(int i=0;i<TopBOW.size();i++)
	{
		cout<<TopBOW[i].second<<" -> "<<TopBOW[i].first<<endl;
	}
	cout<<"reverse"<<endl;*/
	for(int i=TopBOW.size()-1;i>=TopBOW.size()-5;i--)
	{
		cout<<TopBOW[i].second<<" -> "<<TopBOW[i].first<<endl;
	}

}
int main()
{	
	cout<<"Executing"<<endl;
	LeerStopWords();
	DIR *dir;
	dirent *ent;
	//string ruta="/home/karelia/Documentos/2018/TopicosGrafica/MY WORD CLOUD/CORPUS/AP_BBC_CNN_Reuters_nosource_nodate_novo_nofull";
	//if (DIR* dir = opendir("AP_BBC_CNN_Reuters_nosource_nodate_novo_nofull"))
	if (DIR* dir = opendir(ruta.c_str()))
	{   //cout<<"Entre AQUI"<<endl;
 		while (dirent* ent = readdir(dir) )
 			{	int longitud = strlen(ent->d_name); 
 				if(longitud>2)
 				{
 					//cout <<"VOY EN: "<< ent->d_name << '\n';
 					LeerArchivo(ent->d_name);
 				}
 			}
    	//std::cout << ent->d_name << '\n';
  		closedir (dir);
	}

	LlenarMatriz();
	
	bow.close();

	return 0;
}
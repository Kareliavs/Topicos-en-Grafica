#include <bits/stdc++.h>
#include <dirent.h>
#include <regex>
using namespace std;
///compila asi ---> g++ filtro.cpp -o a
ofstream bow("BOW.txt"); 

map <string, int>  BagOfWords;
map <string, int>  NumDocPal;
map <string,  map <string, double> >  matriz;
vector<vector<double> > vect;
map< string , int >::iterator iter,iter2;

struct Documento {
        string nombre;
        int numPal;
        map <string, int> BOW_propio;
};
vector<Documento> Corpus;

int ActualizarBOW(string palabra)
{   //cout<<palabra<< "TAM "<<BagOfWords.size()<<endl;
		
  /*for(iter=BagOfWords.begin(); iter!=BagOfWords.end();iter++)
		{	if(iter->first==palabra) 
			{
				BagOfWords[palabra]++;
				return 0;
			}
			
		}*/
/*cout<<"Esto hay "<<BagOfWords[palabra]<<endl;
		if(BagOfWords[palabra]!=1) */BagOfWords[palabra]++;
		//BagOfWords.push_back(make_pair(palabra,1));
		//BagOfWords.insert(std::pair<string, int>(palabra,1));
	
}
map <string, int>  LeerArchivo(string doc) {
    ifstream fichero; //variable fichero para leer los datos
    fichero.open((doc));
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
				    	num++;
				   		ActualizarBOW(token);
				    	BagOfWords_Doc[token]++;
				    	cout<<" "<<token<<", ";
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

			TF=(double)iter->second/(double)Corpus[i].numPal;
			IDF=log((double)Corpus.size()/(double)NumDocPal[iter->first]);
			matriz[Corpus[i].nombre][iter->first]=(double)TF*(double)IDF;
			//cout<<"THIS IS: "<<TF<<" ="<<iter->second<<" / "<<Corpus[i].numPal<<" = "<<(double)iter->second/(double)Corpus[i].numPal<<" and "<<IDF<<endl;
			
		}
		vector<double> vect1;
		for(iter=BagOfWords.begin(); iter!=BagOfWords.end();iter++)
		{//cout<<"THIS: "<<matriz[Corpus[i].nombre][iter->first]<<endl;
			if(matriz[Corpus[i].nombre][iter->first]!=0)
			{
				vect1.push_back(matriz[Corpus[i].nombre][iter->first]);
			}
			else vect1.push_back(0);
		}
		vect.push_back(vect1);
		
	}

	//////
	cout<<endl<<endl;
	for(iter=BagOfWords.begin(); iter!=BagOfWords.end();iter++)
		{	cout<<iter->first<<"\t";
			bow<<iter->first<<"\t";
		}
	for(int i=0;i<vect.size();i++)
	{	cout<<Corpus[i].nombre<<"\t";
		bow<<Corpus[i].nombre<<"\t";
		for(int j=0;j<vect[i].size();j++)
		{
			cout<<vect[i][j]<<"\t";
			bow<<vect[i][j]<<"\t";
		}
		cout<<endl;
		bow<<endl;
	}

}
int main()
{	
	DIR *dir;
	dirent *ent;
	//string ruta="/home/karelia/Documentos/2018/TopicosGrafica/MY WORD CLOUD/CORPUS/AP_BBC_CNN_Reuters_nosource_nodate_novo_nofull";
	if (DIR* dir = opendir("/home/karelia/Documentos/2018/TopicosGrafica/MY WORD CLOUD/CORPUS/AP_BBC_CNN_Reuters_nosource_nodate_novo_nofull"))
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

#include <bits/stdc++.h>

using namespace std;


bool sortbyasc(const pair<int,int> &a,
              const pair<int,int> &b)
{
    return (a.second > b.second);
}



string cifra(string &mensagem, string &chave){
    string mensagem_cifrada;
    transform(mensagem.begin(),mensagem.end(),mensagem.begin(),::tolower);
    transform(chave.begin(),chave.end(),chave.begin(),::tolower);
    mensagem_cifrada += mensagem;


    for(int i = 0, i_chave = 0;i<mensagem.length();i++, i_chave++){
        char letra = mensagem_cifrada[i];
        if(letra <= 'z' and letra >= 'a'){
            mensagem_cifrada[i] = (mensagem[i] + chave[i_chave%(chave.length())] - 'a'*2)%26 +'a';
        }else i_chave--;
    }


    return mensagem_cifrada;

}


bool isSpecialCharacter(char &c){
    if(c < 'a' or c > 'z'){
        return true;
    }
    return false;
}

bool containsSpecialCharacter(string &s){
    for(int i = 0;i<s.size();i++){
        if(isSpecialCharacter(s[i])){
            return true;
        }
    }
    return false;
}

int nSpecialCharacter(string s){
    int n = 0;
    cout << s << "\n";
    for(int i = 0;i<s.size();i++){
        if(isSpecialCharacter(s[i])){
            n++;
        }
    }
    return n;
}

int correctSizeJump(string &s, int key_size, int comeco){
    int n = key_size;

    for(int i = comeco;i<n + comeco && i<s.size();i++){
        char letra = s[i];
        if(isSpecialCharacter(s[i])){
            n++;
        }
    }

    while(isSpecialCharacter(s[comeco+n])){
        n++;
    }


    return n;
}



int spacing(string &mensagem, int index, int size){

    string trigrama = mensagem.substr(index,size);

    if(containsSpecialCharacter(trigrama)){
        return -1;
    }

    int especiais = 0;
    for(int i = index + size;i<mensagem.length();i++){
        if(isSpecialCharacter(mensagem[i])){
            especiais++;
        }

        if(mensagem.substr(i,size) == trigrama){
            return i - index - especiais; 
        }
    }
    return -1;
}

vector<int> find_key_size(string &mensagem){
    vector<pair<string,int>> espacamento_trigramas;
    vector<pair<int,int>> most_likely(1000,{0,0});

    for(int i = 0; i<mensagem.length();i++){
        int espacamento = spacing(mensagem,i,3);
        if(espacamento!=-1){ 
            string trigrama = mensagem.substr(i,3);
            espacamento_trigramas.push_back({trigrama,espacamento});
        }
    }

    for(int i = 0;i<espacamento_trigramas.size();i++){
        for(int j = 2;j<most_likely.size();j++){
            if(espacamento_trigramas[i].second%j == 0){
                most_likely[j].first = j;
                most_likely[j].second++;
                
            }
        }
    }

    sort(most_likely.begin(),most_likely.end(),sortbyasc);

    vector<int> arr(5);
    for(int i = 0;i<arr.size();i++){
        arr[i] = most_likely[i].first;
    }

    return arr;
    
}



string decifra(string &mensagem_cifrada, string &key){
    transform(mensagem_cifrada.begin(),mensagem_cifrada.end(),mensagem_cifrada.begin(),::tolower);
    transform(key.begin(),key.end(),key.begin(),::tolower);
    string mensagem_decifrada = mensagem_cifrada;
    int key_index = 0;
    for(int i = 0;i<mensagem_cifrada.length();i++, key_index++){
        if(isSpecialCharacter(mensagem_cifrada[i])){
            key_index--;
            mensagem_decifrada[i] = mensagem_cifrada[i];
            continue;
        }

        int shift = (mensagem_cifrada[i] - key[key_index%(key.length())]);
        if(shift<0) shift = 26 + shift;
        mensagem_decifrada[i] = ('a' + shift);
    }
    return mensagem_decifrada;
}


void divide_vector(vector<double> &vetor,int divisor){
    for(int i = 0;i<vetor.size();i++){
        vetor[i] /= divisor;
    }
}

double multiplica_vetores(vector<double> &vetor1,vector<double> &vetor2){
    double result = 0;
    for(int i = 0;i<vetor1.size();i++){
        result += vetor1[i]*vetor2[i];
    }
    return result;
}

int best_shift(vector<double> &language_freq,vector<double> &letters_freq){
    double shift_result = 0;
    int shift_number = 0;
    for(int i = 0;i<language_freq.size();i++){
        double aux = shift_result;
        shift_result = max(multiplica_vetores(language_freq,letters_freq),shift_result);
        if(aux!=shift_result)shift_number=i;

        rotate(letters_freq.begin(),letters_freq.begin()+1,letters_freq.end());
    }
    return shift_number;
}


string find_key(string &mensagem, vector<double> &language_freq, int key_size){
    string key;
    vector<double> letters_freq(26,0);

    
    int aux = key_size;
    for(int i = 0;i<aux;i++){
        int n_letters = 0;

        for(;isSpecialCharacter(mensagem[i]);i++,aux++);
        

        for(int j = i;j<mensagem.length();j+= correctSizeJump(mensagem,key_size, j),n_letters++){
            int letter_number = mensagem[j] - 'a';
            letters_freq[letter_number] += 1;
        }
        divide_vector(letters_freq,n_letters);


        char best_letter = best_shift(language_freq,letters_freq) + 'a';
        key += best_letter; 
    }
    return key;
}

bool chave_duplicada(string &chave_maior, string &chave_menor){
    int s1,s2;
    s1 = chave_maior.size();
    s2 = chave_menor.size();

    if(s1 == s2 || (s1 == 0 || s2 == 0) || (s1%s2 != 0 && s2%s1 != 0)){
        return false;
    }


    for(int i = 0;i<chave_maior.size();i++){
        if(chave_maior[i] != chave_menor[i%chave_menor.size()]){
            return false;
        }
    }
    return true;
}


pair<string,double> most_likely_key(vector<int> &possible_sizes, vector<double> &language_freq, string &mensagem_cifrada){
    double probabilidade = 0;
    vector<double> letters_freq(26,0);
    string desired_key;
    for(int i = 0;i<possible_sizes.size();i++){
        string chave = find_key(mensagem_cifrada,language_freq,possible_sizes[i]);
        string mensagem_decifrada = decifra(mensagem_cifrada, chave);
        int n_letters = 0;
        double probabilidade_aux;

        for(int i = 0;i<mensagem_decifrada.size();i++){
            if(!isSpecialCharacter(mensagem_decifrada[i])){
                n_letters++;
                int letra = mensagem_decifrada[i] - 'a';
                letters_freq[letra]++;
            }
        }


        divide_vector(letters_freq,n_letters);
        
        probabilidade_aux = multiplica_vetores(letters_freq,language_freq);
        if(probabilidade_aux>probabilidade){
            probabilidade = probabilidade_aux;

            if(chave_duplicada(chave, desired_key)){
                if(chave.size() < desired_key.size()){
                    desired_key = chave;
                }
            }else{
                desired_key = chave;
            }

        }


    }

    return {desired_key,probabilidade};
}

string quebrar_cifra(string &mensagem_cifrada){
    string chave;
    vector<double> freq_ingles = {8.167, 1.492,2.782,4.253,12.702,2.228,2.015,6.094,6.966,0.153,0.772,4.025,2.406,6.749,7.507,1.929,0.095,5.987,6.327,9.056,2.758,0.978,2.36,0.15,1.974,0.074};
    divide_vector(freq_ingles,100);

    vector<double> freq_portugues = {14.63, 1.04,3.88,4.99,12.57,1.02,1.3,1.28,6.18,0.4,0.02,2.78,4.74,5.05,10.73,2.52,1.2,6.53,7.81,4.34,4.63,1.67,0.01,0.21,0.01,0.47};
    divide_vector(freq_portugues,100);

    vector<vector<double>> language_freq = {freq_ingles,freq_portugues};

    transform(mensagem_cifrada.begin(),mensagem_cifrada.end(),mensagem_cifrada.begin(),::tolower);


    vector<int> possible_sizes = find_key_size(mensagem_cifrada);

    cout << "Chaves possiveis: ";
    for(int i = 0;i<language_freq.size();i++){
        for(int j = 0;j<possible_sizes.size();j++){
            chave = find_key(mensagem_cifrada,language_freq[i],possible_sizes[j]);
            cout << chave << ", ";
        }
    }
    cout << "\n";

    double probabilidade = 0;
    for(int i = 0;i<language_freq.size();i++){
        pair<string,double> result = most_likely_key(possible_sizes, language_freq[i], mensagem_cifrada);
        if(result.second > probabilidade){
            probabilidade = result.second;
            chave = result.first;
        }
    }
    return chave;
}



int main(){
    string mensagem,mensagem_cifrada,chave,mensagem_decifrada;
    int opcao;
    cout << "Digite o numero da opcao desejada:\n";
    cout << "1: Cifrar\n2: Decifrar\n3: Quebrar cifra\n";

    cin >> opcao;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    if(opcao == 1){
        string chave, mensagem ,mensagem_cifrada;
        cout << "Digite o texto a ser cifrado:\n";
        getline(cin,mensagem);
        
        cout << "Digite a chave:\n";
        getline(cin,chave);

        mensagem_cifrada = cifra(mensagem,chave);

        cout << mensagem_cifrada;
    }else if(opcao == 2){
        string mensagem_cifrada, chave,mensagem_decifrada;

        cout << "Digite a mensagem cifrada:\n";
        getline(cin,mensagem_cifrada);
        cout << "\n";

        cout << "Digite a chave:\n";
        getline(cin,chave);
        cout << "\n";

        mensagem_decifrada = decifra(mensagem_cifrada,chave);
        cout << "Mensagem decifrada:\n\n" << mensagem_decifrada;
    }else if(opcao == 3){
        string mensagem_cifrada, chave,mensagem_decifrada;
        cout << "Digite a mensagem cifrada:\n";
        getline(cin,mensagem_cifrada);
        cout << "\n";
        
        chave = quebrar_cifra(mensagem_cifrada);
        mensagem_decifrada = decifra(mensagem_cifrada,chave);
        cout << "Chave mais provavel: " << chave << "\n\n";
        cout << "Mensagem decifrada com chave acima:\n\n" << mensagem_decifrada;
    }else{
        cout << "Opcao invalida\n";
    }


    return 0;
}

string msg = "Na tranquila cidade de Oaksville, o sol se punha lentamente no horizonte, tingindo o céu com tons de laranja e rosa. As crianças brincavam no parque, rindo e correndo enquanto os adultos aproveitavam a calma do final de tarde. O aroma de comida caseira pairava no ar, anunciando um jantar reconfortante.Enquanto isso, na biblioteca local, um grupo de leitores ávidos se reunia para discutir o último livro do autor best-seller. As páginas viravam freneticamente, revelando segredos entrelaçados em tramas emocionantes.No centro da cidade, uma pequena loja de antiguidades exibia tesouros de tempos passados, despertando a curiosidade dos visitantes. E ao longo do rio sereno que cortava a cidade, os patos nadavam graciosamente, criando círculos na água.Oaksville era um refúgio de tranquilidade e beleza, onde a vida seguia seu ritmo suave, proporcionando a seus habitantes e visitantes um merecido descanso da agitação do mundo moderno.";
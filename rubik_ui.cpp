#include"globals.h"
#include"rubik.h"

static void imbueStream(Stream& IS, const String & V)
{
  String IS_;
  if(IS.good())
  {
    getline(IS,IS_);
  }
  IS.str("");
  IS.clear();
  IS<<V<<' '<<IS_;
}

void Rubik::replaceArguments(String & read_line, const String & arg)
{
  if(arg.find('&')!=String::npos)
  {
    return;
  }
  if(read_line=="&")
  {
    read_line=arg;
  }
  else
  {
    int index=0;
    while((index=read_line.find('&',index))!=String::npos)
    {
      read_line.replace(index,1,arg);
      index+=arg.length();
    }
  }
}

String Rubik::defvar(Stream& IS, const String& fName)
{
  Var_space[fName]="";
  while(IS.good())
  {
    String S;
    IS>>S;
    Var_space[fName]+=S+' ';
  }
  return fName;
}

String Rubik::assoc(Stream& IS)
{
    String A=parser(IS);
    String B=parser(IS);
    return (A=="" || B=="") ? A+B : A==B ? A : A+"->"+B;
}

String Rubik::echo(Stream& IS)
{
  String read_in;
  IS>>read_in;
  std::map<String,String>::const_iterator it=Var_space.find(read_in);
  return (it!=Var_space.end() ? it->second : read_in)+' ';
}

String Rubik::mapcar(Stream& IS)
{
  String Result;
  String lambda;
  IS>>lambda;
  Stream buffer(parser(IS));
  while(buffer.good())
  {
    String step_buffer;
    buffer>>step_buffer;
    if(step_buffer=="")
    {
      continue; // don't handle empty tags
    }
    step_buffer=lambda+' '+step_buffer;
    Stream step_stream(step_buffer);
    Result+=parser(step_stream)+' ';
  }
  return Result;
}

String Rubik::select(Stream& IS, bool Inv)
{
  String head;
  IS>>head;
  String read_in="";
  Stream SS(parser(IS));
  while(SS.good())
  {
    String next;
    next=="";
    SS>>next;
    if((Inv && next.find(head)==String::npos) ||
      (!Inv && next.find(head)!=String::npos) )
    {
      read_in+=next+' ';
    }
  }
  return read_in;
}

String Rubik::pathFinder(Stream& IS)
{
  String Result, From=parser(IS), To=parser(IS);
  while(IS.good())
  {
    String segment=findPath(From,To);
    if(segment=="" && From != To)
    {
      Result="";
      break;
    }
    Result+=segment;
    From=To;
    To=parser(IS);
  }
  return Result+findPath(From,To);
}

String Rubik::merge(Stream& IS) 
{
  String Result=parser(IS),A=parser(IS);
  do
  {
    Result=auxiliary::mergeSimplePaths(Result,A);
    A=parser(IS); 
  }while(IS.good());
  return Result;
}

String Rubik::cube(Stream& IS)
{
  String Result;
  C_EACH_FUNC(B_map,b,index)
  {
    Stream S;
    S<<Sidemarks(index).c_str()<<' '<<Sidemarks(*b).c_str();
    Result+=assoc(S)+' ';
  }
  return Result;
}

String Rubik::variable(Stream& IS, const String& R)
{
  String arg;
  if(R.back()=='&')
  {
    IS>>arg; // it's a function. Argumentum is needed
  }
  imbueStream(IS,Var_space.at(R));
  return parser(IS,arg);
}

void Rubik::printSidemarks(Stream& IS)
{
  while(IS.good())
  {
    Sidemarks S= parser(IS);
    if(!S.valid())
    {
      continue;
    }
    OUT(S<<':'<<S.getEigenvalue()<<' ');
  }
  NL_
}

String Rubik::callBruteForce(Stream& IS)
{
  String As=parser(IS);
  String Result;
  while(IS.good())
  {
    Result+=parser(IS)+' ';
  }
  return bruteForce(Result,As);
}

String Rubik::file_open(const char * F)
{
  std::ifstream ifs(F,std::ifstream::in);
  if(ifs.is_open())
  {
    REPL(ifs,std::cout);
  }
  else
  {
    OUT_(NL<<"Something went wrong. Unable to open the file: "<<F)
  }
  OUT_(NL<<"The file "<<F<<" has been closed.\n");
  ifs.close();
  return F;
}

String Rubik::file_open(Stream& IS)
{
  String F;
  IS >> F;
  std::ifstream ifs(F,std::ifstream::in);
  if(ifs.is_open())
  {
    REPL(ifs,std::cout);
  }
  else
  {
    OUT_(NL<<"Something went wrong. Unable to open the file: "<<F)
  }
  OUT_(NL<<"The file "<<F<<" has been closed.\n");
  ifs.close();
  return F;
}
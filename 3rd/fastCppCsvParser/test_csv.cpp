# include "csv.h"
//The libary provides two classes:
//	LineReader:	A class to efficiently read large files line by line.
//	CSVReader:	A class that efficiently reads large CSV files.

# include "iostream"

//其他用法自己实验
//g++ *.cpp -std=c++0x


void test0(){
	std::cout<< std::endl;
	io::LineReader in("test.csv");
	while(char* line = in.next_line()){
		std::cout<< line << std::endl;
	}
}

void test1(){
	io::CSVReader<3> in("test1.csv");
	//ignore_extra_column
	in.read_header(io::ignore_extra_column, "vendor", "speed", "size");
	if(!in.has_column("a") || !in.has_column("b"))
		std::cout<< "don't have column a/b" << std::endl;
	std::string vendor; int size; double speed;
	while(in.read_row(vendor, speed, size)){  
		std::cout<< vendor <<","<< size <<","<< speed << std::endl;
	}
}

void test2(){
	std::cout<< std::endl;
	io::CSVReader<3> in("test2.csv");
	in.read_header(io::ignore_missing_column, "num", "inf", "xxx");
	int a=99, b=99, c = 99;
	while(in.read_row(a,b,c)){
		std::cout<< a <<","<< b <<","<< c << std::endl;
	}
}

int main(){
	test0();
	test1();
	test2();
}

/*
template<
  unsigned column_count,
  class trim_policy = trim_chars<' ', '\t'>, 
  class quote_policy = no_quote_escape<','>,
  class overflow_policy = throw_on_overflow,
  class comment_policy = no_comment
>
class CSVReader{
public:
  // Constructors
  // same as for LineReader

  // Parsing Header
  void read_header(ignore_column ignore_policy, some_string_type col_name1, some_string_type col_name2, ...);
  void set_header(some_string_type col_name1, some_string_type col_name2, ...);
  bool has_column(some_string_type col_name)const;

  // Read
  char*next_line();
  bool read_row(ColType1&col1, ColType2&col2, ...);

  // File Location 
  void set_file_line(unsigned);
  unsigned get_file_line()const;
  void set_file_name(some_string_type file_name);
  const char*get_truncated_file_name()const;
};
*/



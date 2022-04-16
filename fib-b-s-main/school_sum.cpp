#include<bits/stdc++.h>
using namespace std;

//Function for finding sum of 2 large number
/*string SumStr(string str1, string str2)
{

}*/

int main()
{
   string str1 = "8";
   string str2 = "13";
   cout << str1+" has "<<str1.length()<< "digits" << endl;
   cout << str2+" has "<<str2.length()<<"digits" << endl;
// from our school knowledge we only know that we add digit by digit from back to front so we will reverse it
   reverse(str1.begin(), str1.end());//reverse str1 and str2 and assign to first variables
   reverse(str2.begin(),str2.end());
   /*cout << str1[1] << endl;
   cout << str2[0] << endl;
   */
   int n1=str1.length(), n2=str2.length();
//sum digit by digit
   int n;
   if(n1>n2)
   {
      n=n2;
   }else{
      n=n1;
   }
   //define an empty string for assign the value each digit
   string str ="";
   int carry = 0;
   for(int i=0;i<n;i++)
   {
   int sum = ((str1[i]-'0')+(str2[i]-'0')+carry);
   // string-'0' is a conversion from _char_ to _int_
   //focus on current digit+digit+carry from previous sum 
   //cout << sum <<endl;
   str.push_back(sum%10+'0');
   //(int +'0') convert int to str and then stack digit back to str
   carry = sum/10;
   cout << carry << endl;
   //cout<<str<<"push back"<<endl;
   }
   //when the carry is remaining 
   if (carry>0)
   {
   carry=carry+int(str2[n]-'0'); 
   str.push_back(carry+'0');
   }
   //after we finish sum from ending -> we need to reverse everything back
   reverse(str.begin(),str.end());
   cout << str << " has"<< str.length()<< "digits" << endl;
   
   //cout << '0' << "print" <<endl;
   //cout << '\0' << "print" << endl;//'\0' is null
}


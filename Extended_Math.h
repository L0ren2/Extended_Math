// Usage:
// #include "Extended_Math.h"
// int main()
// {
//   std::array<int, 3> x = {1, 2, 3};
//   std::cout << "Arithmetischer Mittelwert: " << empir_mid(x) << std::endl; // #=> Arithmetischer Mittelwert: 2
//   return 0;
// }
//
#pragma once
#include <array>
#include <math.h>
#include <map>
#include <algorithm>

//empirischer / arithmetischer / -mittelwert
template <class T, size_t size>
double empir_mid(const std::array<T, size>& theArray)
{
  double sum = 0;
  for(T d : theArray)
  {
    sum += d;
  }
  return sum / size;
}

//varianz
template <class T, size_t size>
double varianz(const std::array<T, size>& theArray)
{
  double mid = empir_mid<T>(theArray);
  double sum = 0;
  for(T d : theArray)
  {
    sum += (d - mid) * (d - mid);
  }
  return sum / size;
}

template <class T, size_t size>
double std_abweichung(const std::array<T, size>& theArray)
{
  return sqrt(varianz(theArray));
}

//kovarianz zwischen den beiden arrays
template <class T, size_t size>
double kovarianz(const std::array<T, size>& Array1, const std::array<T, size>& Array2)
{
  double mid1 = empir_mid<T>(Array1);
  double mid2 = empir_mid<T>(Array2);

  double sum = 0;
  for(int i = 0; i < Array1.size(); i++)
  {
    sum += (Array1[i] - mid1) * (Array2[i] - mid2);
  }
  return sum / Array1.size();
}

//korrelationskoeffizient zwischen den zufallsgroessen
template <class T, size_t size>
double korkoef(const std::array<T, size>& Array1, const std::array<T, size>& Array2)
{
  double cov = kovarianz(Array1, Array2);
  double svar1=std_abweichung(Array1), svar2=std_abweichung(Array2);
  return cov / (svar1 * svar2);
}

//modalwert des arrays (häufigstes vorkommnis)
//WARNUNG: funktioniert nur für int oder double mit x.0 zuverlässig
template<class T, size_t size>
double modus(const std::array<T, size>& theArray)
{
  //find max
  T max = 0;
  for(T t : theArray)
  {
    if(t > max)
      max = t;
  }
  //make counter array
  int counter_size = static_cast<int>(max+1);
  int* counter_array = new int[counter_size];
  for(int i = 0; i < counter_size; i++)
  {
    counter_array[i] = 0;
  }
  //count occurences
  for(T t : theArray)
  { 
    counter_array[static_cast<int>(t)]++;
  }
  int count = 0;
  int index = 0;
  for(int i = 0; i < counter_size; i++)
  {
    if(counter_array[i] > count)
    {
      count = counter_array[i];
      index = i;
    }
  }
  delete[] counter_array;
  return index;
}

//median des arrays (nach sort in der mitte des arrays)
template<class T, size_t size>
double median(std::array<T, size> theArray)
{
  std::sort(theArray.begin(), theArray.end());
  if(theArray.size() % 2 == 0)
  {
    int mid = theArray.size() / 2 - 1;
    return (static_cast<double>(theArray[mid] + theArray[mid + 1])) / 2.0;
  }
  return theArray[theArray.size()/2];
}

typedef struct Linear
{
  double mx, bx;
  double my, by;
} Linear;

//Least_Squares uses new, so don't forget to delete
template <typename T, size_t size>
Linear* Least_Squares(const std::array<T, size>& X_Array,const std::array<T, size>& Y_Array)
{
  double _X = empir_mid(X_Array);
  double _Y = empir_mid(Y_Array);
  std::array<T, size>E_X;
  std::array<T, size>E_Y;
  double sumXY = 0;
  double sumXX = 0;
  double sumYY = 0;
  for(int i = 0; i < size; i++)
  {
    E_X[i] = X_Array[i] - _X;
    E_Y[i] = Y_Array[i] - _Y;

    sumXY += E_X[i] * E_Y[i];
    sumXX += E_X[i] * E_X[i];
    sumYY += E_Y[i] * E_Y[i];
  }
  Linear* linear = new Linear;
  linear->mx = sumXY / sumXX;
  linear->bx = _Y - linear->mx * _X;

  linear->my = sumXY / sumYY;
  linear->by = _Y - linear->my * _X;
  return linear;
}

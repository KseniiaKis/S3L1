// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int n)
{
  if (n > 0)
  {
    BitLen = n * sizeof(TELEM) * 8;
    arr = new TELEM[n];
    len = n;
    for (int i = 0; i < len; i++)
    {
      arr[i] = 0;
    }
  }
  else
    throw("error");
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  if (bf.BitLen > 0) {
    BitLen = bf.BitLen;
    arr = new TELEM[bf.len];
    for (int i = 0; i < bf.len; i++)
    {
      arr[i] = bf.arr[i];
    }
    len = bf.len;
  }
  else
    throw("error");
}

TBitField::~TBitField()
{
  if (arr != 0) {
    delete[]arr;
    arr = 0;
  }
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  if (n < 0)
    throw "n<0";
  return n>>5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  if (n<0)
    throw "n<0";
  return 1<<(n&31);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return FAKE_INT;
}

void TBitField::SetBit(const int n) // установить бит
{
  if (n >= 0) {
    int i = GetMemIndex(n);
    int m = GetMemMask(n);
    arr[i] = arr[i] | m;
  }
  else
    throw "n<0";
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if (n >= 0) {
    int i = GetMemIndex(n);
    int m = GetMemMask(n);
    arr[i] = arr[i] & m; arr[i] = arr[i] | m;
  }
  else
    throw "n<0";
} 

int TBitField::GetBit(const int n) const // получить значение бита
{
  if ((n < 0) || (n > len))
  {
    throw "error";
  }
  else
  {
    int i = GetMemIndex(n);
    int m = GetMemMask(n);
    return (arr[i] & m);
  };
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  if (this == &bf)
    return *this;
  else
  {
    delete[] arr;
    len = bf.len;
    arr = new TELEM[len];
    BitLen = bf.BitLen;
    for (int i = 0; i < len; i++)
    {
      arr[i] = bf.arr[i];
    }
    return *this;
  }
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  if (len != bf.len || BitLen != bf.BitLen) return 0;
  else
  {
    for (int i = 0; i < len; i++)
    {
      if (arr[i] != bf.arr[i])
        return 0;
    }
    return 1;
  }
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  if (len != bf.len || BitLen != bf.BitLen)
    throw "error";
  TBitField res(*this);
  for (int i = 0; i < len; i++)
  {
    res.arr[i] = this->arr[i] | bf.arr[i];
  }
  return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  if (bf.BitLen > (*this).BitLen) (*this).BitLen = bf.BitLen;
  if ((this->len) < bf.len) {
    TELEM* ptr = (this->arr);
    arr = new TELEM[bf.len];
    for (int i = 0; i < len; i++)
      arr[i] = ptr[i];
    for (int i = len; i < bf.len; i++)
      arr[i] = 0;
    delete[]ptr;
    len = bf.len;
  }
  for (int i = 0; i < bf.len; i++)
    (*this).arr[i] = (*this).arr[i] & bf.arr[i];
  return (*this);
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField mask(BitLen);
  for (int i = 0; i < mask.GetLength(); i++)
    mask.SetBit(i);
  for (int i = 0; i < (*this).len; i++) {
    (*this).arr[i] = ~arr[i];
    (*this).arr[i] = (*this).arr[i] & mask.arr[i];
  }

  return (*this);
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  int n;
  istr >> n;
  TBitField s(n);
  for (int i = 0; i < n; i++) {
    istr >> n;
    if (n == 1)
      s.SetBit(i);
  }
  bf = s;
  return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
  for (int i = 0; i < bf.GetLength(); i++)
    ostr << bf.GetBit(i);
  return ostr;
}

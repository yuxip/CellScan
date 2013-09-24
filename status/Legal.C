Bool_t Legal(Int_t iew,Int_t nstb,Int_t row0,Int_t col0)
{
  if(iew>0 && iew<2)return false;
  if(nstb<1 || nstb>4)return false;
  if(nstb>2)
    {
      if(row0<0 || row0>23)return false;
      if(col0<0 || col0>11)return false;
      if(fabs(1.*row0-11.5)<5 && col0<5)return false;
    }
  else
    {
      if(row0<0 || row0>33)return false;
      if(col0<0 || col0>16)return false;
      if(fabs(1.*row0-16.5)<8 && col0<8)return false;
      if(row0<col0-9.5)return false;
      if(33-row0<col0-9.5)return false;
    };
  return true;
}

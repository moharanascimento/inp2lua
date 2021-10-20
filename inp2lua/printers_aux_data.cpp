#include "printers_aux_data.h"

void printersAuxData::printAuxData(std::ofstream& out)
{
 if (out.is_open())
 {
  out << "-------------------------------------------------------------" << std::endl;
  out << "--  Post process Gid script" << std::endl;
  out << "--  Autor: Julio Rueda" << std::endl;
  out << "-------------------------------------------------------------" << std::endl;
  out << "-------------------------------------------------------------" << std::endl;
  out << "--  Adaptative Time function " << std::endl;
  out << "-- Autor: Julio Rueda" << std::endl;
  out << "-------------------------------------------------------------" << std::endl;
  out << "function adaptativeTime(dt,err,conv,niter,nbroke,solverOptions)" << std::endl;
  out << "  --local hydtol = " << std::endl;
  out << "  local TolPhy  = solverOptions.tolerance[\"mechanic\"] " << std::endl;
  out << "  local ftime = 2" << std::endl;
  out << "  local dtol = (TolPhy-err)/TolPhy*100" << std::endl;
  out << "  if(conv)then" << std::endl;
  out << "      if(dtol < 15 or niter > 5) then" << std::endl;
  out << "        dt = dt" << std::endl;
  out << "      elseif(ftime*dt >= solverOptions.timeMaxIncrement ) then" << std::endl;
  out << "        dt = solverOptions.timeMaxIncrement " << std::endl;
  out << "      elseif(dtol > 0 and dtol < 20) then" << std::endl;
  out << "        dt = 1.2*dt " << std::endl;
  out << "      else" << std::endl;
  out << "        dt = ftime*dt        " << std::endl;
  out << "      end " << std::endl;
  out << "   else" << std::endl;
  out << "      if(nbroke - 1 < solverOptions.attemptMax) then" << std::endl;
  out << "        dt = dt/4" << std::endl;
  out << "      elseif(nbroke > solverOptions.attemptMax)then" << std::endl;
  out << "        dt = solverOptions.timeMinIncrement" << std::endl;
  out << "      end  " << std::endl;
  out << "   end" << std::endl;
  out << "   --io.print(tr('DTol = %1 %') :num(dtol))      " << std::endl;
  out << "   return dt" << std::endl;
  out << "end" << std::endl;
 }
}
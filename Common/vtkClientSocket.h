/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkClientSocket.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkClientSocket - Encapsulates a client socket.

#ifndef __vtkClientSocket_h
#define __vtkClientSocket_h

#include "vtkSocket.h"
class vtkServerSocket;

class VTK_COMMON_EXPORT vtkClientSocket : public vtkSocket
{
public:
  static vtkClientSocket* New();
  vtkTypeMacro(vtkClientSocket, vtkSocket);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Connects to host. Returns 0 on success, -1 on error.
  int ConnectToServer(const char* hostname, int port); 

  // Description:
  // Connects to host.  Returns 0 on success, -1 on error.  If silenceError
  // is true, this method will not print an error message if the connection
  // to hostname:port fails.
  int ConnectToServer(const char* hostname, int port, bool silenceError);
 
  // Description:
  // Returns if the socket is on the connecting side (the side that requests a
  // ConnectToServer() or on the connected side (the side that was waiting for
  // the client to connect). This is used to disambiguate the two ends of a socket
  // connection.
  vtkGetMacro(ConnectingSide, bool);

//BTX
protected:
  vtkClientSocket();
  ~vtkClientSocket();

  vtkSetMacro(ConnectingSide, bool);
  bool ConnectingSide;
  friend class vtkServerSocket;
private:
  vtkClientSocket(const vtkClientSocket&); // Not implemented.
  void operator=(const vtkClientSocket&); // Not implemented.
//ETX
};


#endif


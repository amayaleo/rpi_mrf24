

#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <cstring>
#include <termios.h>
#include <security/src/security.h>


namespace SECURITY{

// Función para ocultar la entrada del usuario
  std::string Security_t::getHiddenInput() 
  {
      std::string input;
      struct termios oldt, newt;

      // Guardar configuración actual del terminal
      tcgetattr(0, &oldt); // Puedes usar 0 directamente en lugar de STDIN_FILENO
      newt = oldt;

      // Desactivar la impresión en la pantalla
      newt.c_lflag &= ~ECHO;

      // Aplicar la nueva configuración
      tcsetattr(0, TCSANOW, &newt);

      // Leer la entrada del usuario
      std::getline(std::cin, input);

      // Restaurar la configuración original del terminal
      tcsetattr(0, TCSANOW, &oldt);

      return input;
  }

  // Función para calcular el hash SHA-256
  std::string Security_t::sha256(const std::string& str) 
  {
      unsigned char hash[SHA256_DIGEST_LENGTH];
      SHA256_CTX sha256;
      SHA256_Init(&sha256);
      SHA256_Update(&sha256, str.c_str(), str.size());
      SHA256_Final(hash, &sha256);

      std::stringstream ss;
      for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
          ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
      }
      return ss.str();
  }


  int Security_t::function() {
      std::string variable;
  //    std::cout << sha256("0") << '\n';

      std::cout << "Ingrese el password: ";
      variable = getHiddenInput();

      auto result = sha256("0") == sha256(variable);
  if (result==1){
    std::cout << "\nsuccess\n";
  return 0;
  }
    std::cout << "\nerror\n";
      //?out << "result: " << result << "\n";
      return 0;
  }

}
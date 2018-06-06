//
// Created by 金乐盛 on 2018/5/25.
//

#ifndef CONSUMER_CONSUMERCLASS_H
#define CONSUMER_CONSUMERCLASS_H
#include "bppair.h"
using namespace tool;
namespace consumer_system{
class consumer{
  public:
    explicit consumer(int id = 0, const char* name = "", const char* password = "", const char* email = "", const char* phone = "", int ma = 1):id_consumer(id), name_consumer(name),
                                                                               password_consumer(password), email_consumer(email), phone_consumer(phone), is_manager(ma){
    }

    int get_id_consumer(){
        return id_consumer;
    }
    mychar get_name(){
        return name_consumer;
    }
    mychar get_password(){
        return password_consumer;
    }
    mychar get_email(){
        return email_consumer;
    }

    mychar get_phone(){
        return phone_consumer;
    }

    int get_is_manager(){
        return is_manager;
    }

  private:
    int id_consumer;
    int is_manager;
    mychar name_consumer;
    mychar password_consumer;
    mychar email_consumer;
    mychar phone_consumer;
};
}
#endif //CONSUMER_CONSUMERCLASS_H

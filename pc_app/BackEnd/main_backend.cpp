#include <iostream>
#include "firebase/app.h"
#include "firebase/firestore.h"

using firebase::App;
using firebase::AppOptions;
using firebase::firestore::DocumentReference;
using firebase::firestore::FieldValue;
using firebase::firestore::Firestore;

int main()
{
    AppOptions options;
    options.set_project_id("your-project-id");
    options.set_api_key("your-api-key");
    options.set_app_id("your-app-id");

    App *app = App::Create(options);
    Firestore *db = Firestore::GetInstance(app);

    if (db != nullptr)
    {
        std::cout << "Connected to Firebase!" << std::endl;

        DocumentReference doc_ref = db->Collection("test_menu").Document("item_01");

        doc_ref.Set({{"name", FieldValue::String("Som Tum Thai")},
                     {"status", FieldValue::String("Testing Connection")}})
            .OnCompletion([](const firebase::Future<void> &future)
                          {
            if (future.error() == firebase::firestore::Error::kErrorOk) {
                std::cout << "Write Success: Data is now in Firebase!" << std::endl;
            } else {
                std::cout << "Write Failed: " << future.error_message() << std::endl;
            } });
    }

    std::cout << "Wait for result... (Press Enter to exit)" << std::endl;
    std::cin.get();

    delete app;
    return 0;
}
def decrypt_message(encrypted_text):
    decrypted_message = ""
    for char in encrypted_text:
        if char.isalpha():
            if char.islower():  
                decrypted_message += chr(ord('z') - (ord(char) - ord('a')))
            elif char.isupper(): 
                decrypted_message += chr(ord('Z') - (ord(char) - ord('A')))
        else:
            decrypted_message += char  

    return decrypted_message

encrypted_text = "r olev zorxv"
print(decrypt_message(encrypted_text))

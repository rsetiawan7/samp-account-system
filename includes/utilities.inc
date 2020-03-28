native WP_Hash(buffer[], len, const str[]);

#if !defined isnull
  #define isnull(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

GenerateRandomString(strDest[], strLen = 10)
{
    while(strLen--) {
      strDest[strLen] = random(2) ? (random(26) + (random(2) ? 'a' : 'A')) : (random(10) + '0');
    }
}

HashPassword(const password[], const salt[]) {
    static
        // A string that ready to hash.
        ConcatenatedStr[(MAX_SALT_LENGTH * 2) + MAX_PASSWORD_LENGTH + 1],
        // Hashed password from input password, global salt, and its own salt.
        CurrentHash[WHIRLPOOL_LENGTH + 1];

    // Concat all salts and its plain password into a string.
    format(ConcatenatedStr, sizeof(ConcatenatedStr), "%s%s%s", salt, gSalt, password);
    // Hash the concatenated string.
    WP_Hash(CurrentHash, sizeof(CurrentHash), ConcatenatedStr);

    return CurrentHash;
}
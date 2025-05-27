db.createUser({
  user: 'game',
  pwd: 'password123',
  roles: [
    {
      role: 'readWrite',
      db: 'game'
    }
  ]
})

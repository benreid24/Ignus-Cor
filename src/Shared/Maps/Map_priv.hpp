/**
 * Adds a new layer at the specified position
 */
void addLayer(int i = -1);

/**
 * Deletes the given layer
 */
void removeLayer(int i);

/**
 * Returns the total number of layers
 */
int getLayerCount();

/**
 * Sets the first y-sort layer
 */
void setFirstYSortLayer(int i);

/**
 * Returns the first y-sort layer
 */
int getFirstYSortLayer();

/**
 * Sets the first top layer
 */
void setFirstTopLayer(int i);

/**
 * Returns the first top layer
 */
int getFirstTopLayer();

/**
 * Returns a reference to the filename of the playlist to use
 */
std::string& getMusic();

/**
 * Returns a reference to the string for the load script
 */
std::string& getLoadScript();

/**
 * Returns a reference to the string for the unload script
 */
std::string& getUnloadScript();

/**
 * Returns a reference to the frequency of the weather
 */
int& getWeatherFrequency();

/**
 * Resizes the map
 *
 * \param sz The new size
 * \param useTop True to crop/add to/from the top when resizing the y dimension
 * \param useLeft True to crop/add to/from the left when resizing the x dimension
 */
void resize(sf::Vector2i sz, bool useTop, bool useLeft);

/**
 * Modifies the world collision map
 *
 * \param x The x coordinate of the collision to change
 * \param y The y coordinate of the collision to change
 * \param c The type of collision to set there (0 = none, 1 = all, 2 = top, 3 = right, 4 = bottom, 5 = left, 6 = topRight, 7 = bottomRight, 8 = bottomLeft, 9 = topLeft, 10 = topBottom, 11 = rightLeft, 12 = noTop, 13 = noRight, 14 = noBottom, 15 = noLeft)
 */
void setCollision(int x, int y, int c);

/**
 * Returns the collision at the specified position
 */
int getCollision(int x, int y);

/**
 * Adds a light to the world
 *
 * \param l The Light to add
 */
void addLight(Light l);

/**
 * Returns a pointer to the light at the given location
 */
Light* getLight(int x, int y);

/**
 * Removes the light at the given position from the world
 *
 * \param x The x position of the light to remove
 * \param y The y position of the light to remove
 */
void removeLight(int x, int y);

/**
 * Removes all lights from the world
 */
void removeAllLights();

/**
 * Adds the given EntitySpawn
 */
void addEntitySpawn(EntitySpawn ps);

/**
 * Returns a pointer to the EntitySpawn at the given position, nullptr if none
 */
EntitySpawn* getEntitySpawn(int x, int y);

/**
 * Removes the EntitySpawn at the given position
 */
void removeEntitySpawn(int x, int y);

/**
 * Adds the given MapSpawner to the map
 */
void addMapSpawner(MapSpawner sp);

/**
 * Returns a pointer to the MapSpawner at the given position, nullptr if none present
 */
MapSpawner* getMapSpawner(int x, int y);

/**
 * Removes the MapSpawner at the given position
 */
void removeMapSpawner(int x, int y);

/**
 * Adds the given item to the map
 */
void addItem(int itemId, sf::Vector2i position);

/**
 * Returns a pointer to the item at the given position, nullptr if none
 */
MapItem* getItem(sf::Vector2i position);

/**
 * Updates the given MapItem
 */
void updateItem(MapItem* orig);

/**
 * Removes the item at the given position
 */
void removeItem(sf::Vector2i position);

/**
 * Sets the ambient lighting to the given value, rather than letting it be determined by the time of day. Use when inside
 *
 * \param o The light level [0,255]. Use 255 to let time control the ambient light level
 */
void setLightingOverride(int o);

/**
 * Returns the current light override
 */
int getLightOverride();

/**
 * Edits the tile at the given position to the new id
 *
 * \param x The x position of the tile
 * \param y The y position of the tile
 * \param layer The layer of the tile
 * \param nId The new id to set the tile to
 * \param isAnim Whether or not the new tile should be an animation or image
 */
void editTile(int x, int y, int layer, int nId, bool isAnim);

/**
 * Syncs up the internal animation table with the tileset. Use this when adding/removing tiles from the tileset
 */
void syncAnimTable();

/**
 * Removes all references to tiles/animations that no longer exist in the tileset. Call this after deleting a tile/animation
 */
void clearBrokenTiles();

/**
 * Adds the given MapEvent to the map
 */
void addEvent(MapEvent event);

/**
 * Removes the Event at the given position
 */
void removeEvent(int x, int y);

/**
 * Returns a pointer to the MapEvent at the given position, or nullptr if none found
 */
MapEvent::Ptr getEvent(int x, int y);

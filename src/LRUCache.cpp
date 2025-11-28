#include "LRUCache.h"
#include <iostream>

LRUCache::LRUCache(size_t capacity)
    : slots(capacity), max_size(capacity), access_counter(0) {}

bool LRUCache::contains(const std::string& track_id) const {
    return findSlot(track_id) != max_size;
}

AudioTrack* LRUCache::get(const std::string& track_id) {
    size_t idx = findSlot(track_id);
    if (idx == max_size) return nullptr;
    return slots[idx].access(++access_counter);
}

/**
 * TODO: Implement the put() method for LRUCache
 */
bool LRUCache::put(PointerWrapper<AudioTrack> track) {
    //return false; // Placeholder

    //ADDED HERE!!!!!!

    if(!track){
        return false;
    }

    if(contains(track->get_title())){
        size_t idx = findSlot(track->get_title());
        slots[idx].access(++access_counter);
        return false;
    }

    bool evictedSlot = false;
    if(size() == capacity()){
        evictedSlot = evictLRU();
    }

    size_t idx = findEmptySlot();
    slots[idx].store(std::move(track), ++access_counter);
    return evictedSlot;
}

bool LRUCache::evictLRU() {
    size_t lru = findLRUSlot();
    if (lru == max_size || !slots[lru].isOccupied()) return false;
    slots[lru].clear();
    return true;
}

size_t LRUCache::size() const {
    size_t count = 0;
    for (const auto& slot : slots) if (slot.isOccupied()) ++count;
    return count;
}

void LRUCache::clear() {
    for (auto& slot : slots) {
        slot.clear();
    }
}

void LRUCache::displayStatus() const {
    std::cout << "[LRUCache] Status: " << size() << "/" << max_size << " slots used\n";
    for (size_t i = 0; i < max_size; ++i) {
        if(slots[i].isOccupied()){
            std::cout << "  Slot " << i << ": " << slots[i].getTrack()->get_title()
                      << " (last access: " << slots[i].getLastAccessTime() << ")\n";
        } else {
            std::cout << "  Slot " << i << ": [EMPTY]\n";
        }
    }
}

size_t LRUCache::findSlot(const std::string& track_id) const {
    for (size_t i = 0; i < max_size; ++i) {
        if (slots[i].isOccupied() && slots[i].getTrack()->get_title() == track_id) return i;
    }
    return max_size;

}

/**
 * TODO: Implement the findLRUSlot() method for LRUCache
 */
size_t LRUCache::findLRUSlot() const {
    //return 0; // Placeholder

    //ADDED HERE!!!!!!
    size_t min_index = max_size;

    for (size_t i = 0; i < max_size; ++i) {
        if (slots[i].isOccupied()) {
            min_index = i;
            break;
        }
    }

    if (min_index == max_size) {
        return max_size;
    }

    for (size_t i = min_index + 1; i < max_size; ++i) {
        if (slots[i].isOccupied() &&
            slots[i].getLastAccessTime() < slots[min_index].getLastAccessTime()) {

            min_index = i;
        }
    }

    return min_index; //if cache is empty

}

size_t LRUCache::findEmptySlot() const {
    for (size_t i = 0; i < max_size; ++i) {
        if (!slots[i].isOccupied()) return i;
    }
    return max_size;
}

void LRUCache::set_capacity(size_t capacity){
    if (max_size == capacity)
        return;
    //udpate max size
    max_size = capacity;
    //update the slots vector
    slots.resize(capacity);
}
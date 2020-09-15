#include <cmath>
#include <iomanip>
#include <map>
#include <vector>
#include <cstdint>

#ifndef PRINT_BST_H
#define PRINT_BST_H
#define PPBST_MAX_HEIGHT 6
template<typename Key, typename Value>
int getNodeDepth(BinarySearchTree<Key, Value> const & tree, Node<Key, Value> * root, Node<Key, Value> * node)
{
    int dist = 1;

    while(node != root)
    {
        if(node == nullptr)
        {
            return -2;
        }

        ++dist;
        node = node->getParent();

        if(dist > PPBST_MAX_HEIGHT)
        {
            return -1;
        }
    }

    return dist;
}
template<typename Key, typename Value>
int getSubtreeHeight(Node<Key, Value> * root, int recursionDepth = 1)
{
    if(root == nullptr)
    {
        return 0;
    }

    if(recursionDepth > PPBST_MAX_HEIGHT)
    {
        return 0;
    }
    return std::max(getSubtreeHeight(root->getLeft(), recursionDepth + 1),
                    getSubtreeHeight(root->getRight(), recursionDepth + 1)) + 1;
}



template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::printRoot (Node<Key, Value>* root) const
{
    if(root == nullptr)
    {
        std::cout << "<empty tree>" << std::endl;
        return;
    }

#define BOX_WIDTH 4
#define PADDING 2
#define ELEMENT_WIDTH (BOX_WIDTH + PADDING)
    std::ios::fmtflags origCoutState(std::cout.flags());
    uint32_t printedTreeHeight = getSubtreeHeight(root);
    bool clippedFinalElements = false;
    if(printedTreeHeight > PPBST_MAX_HEIGHT)
    {
        printedTreeHeight = PPBST_MAX_HEIGHT;
        clippedFinalElements = true;

    }

    uint16_t finalRowNumElements = (uint16_t)std::pow(2, printedTreeHeight - 1);
    uint16_t finalRowWidth = ((uint16_t)(ELEMENT_WIDTH * finalRowNumElements - PADDING));
    std::map<Key, uint8_t> valuePlaceholders;

    uint8_t nextPlaceHolderVal = 1;
    for(typename BinarySearchTree<Key, Value>::iterator treeIter = this->begin(); treeIter != this->end(); ++treeIter)
    {

        if(getNodeDepth(*this, root, treeIter.current_) != -1)
        {
            valuePlaceholders.insert(std::make_pair(treeIter->first, nextPlaceHolderVal++));
        }

    }
    uint16_t firstElementMargin = ((uint16_t)(finalRowWidth/2 - (BOX_WIDTH/2)));

    uint16_t elementPadding = ((uint16_t)(finalRowWidth - 2));

    std::vector<Node<Key, Value> *> currRowNodes;
    currRowNodes.push_back(root);

    for(size_t levelIndex = 0; levelIndex < printedTreeHeight; ++levelIndex)
    {
        uint16_t numElements =(uint16_t)std::pow(2, levelIndex);
        std::cout << std::string(firstElementMargin, ' ');
        for(size_t elementIndex = 0; elementIndex < numElements; ++elementIndex)
        {
            if(currRowNodes[elementIndex] == nullptr)
            {
                std::cout << "    ";
            }
            else
            {
                uint16_t placeholder = valuePlaceholders[currRowNodes[elementIndex]->getItem().first];
                std::cout << "[" << std::setfill('0') << std::setw(2) << placeholder << "]";
            }

            if(elementIndex != ((uint16_t)(numElements - 1)))
            {
                std::cout << std::string(elementPadding, ' ');
            }
        }
        std::cout << std::endl;
        elementPadding = ((uint16_t)((elementPadding - BOX_WIDTH) / 2));
        firstElementMargin = ((uint16_t)(firstElementMargin - (elementPadding / 2 + 2)));
        std::vector<Node<Key, Value> *> prevRowNodes = currRowNodes;
        currRowNodes.clear();
        for(typename std::vector<Node<Key, Value> *>::iterator prevRowIter = prevRowNodes.begin(); prevRowIter != prevRowNodes.end() ; ++prevRowIter)
        {
            if(*prevRowIter == nullptr)
            {
                currRowNodes.push_back(nullptr);
                currRowNodes.push_back(nullptr);
            }
            else
            {
                currRowNodes.push_back((*prevRowIter)->getLeft());
                currRowNodes.push_back((*prevRowIter)->getRight());
            }
        }
        if(levelIndex < printedTreeHeight - 1)
        {
            std::cout << std::string(firstElementMargin + 2, ' ');

            for(size_t prevRowElementIndex = 0; prevRowElementIndex < prevRowNodes.size(); ++prevRowElementIndex)
            {
                Node<Key, Value> * currNode = prevRowNodes[prevRowElementIndex];
                if(currNode == nullptr || currNode->getLeft() == nullptr)
                {
                    std::cout << std::string(elementPadding/2 + 3, ' ');
                }
                else
                {
                    std::cout << "\u250c";

                    for(int numLines = 0; numLines < (elementPadding/2 - 1); ++numLines)
                    {
                        std::cout << u8"\u2500";
                    }

                    std::cout << "\u2518  ";
                }
                if(currNode == nullptr || currNode->getRight() == nullptr)
                {
                    std::cout << std::string(elementPadding/2 + 3, ' ');
                }
                else
                {
                    std::cout << "\u2514";

                    for(int numLines = 0; numLines < (elementPadding/2 - 1); ++numLines)
                    {
                        std::cout << u8"\u2500";
                    }

                    std::cout << "\u2510  ";
                }

                std::cout << std::string(elementPadding + 2, ' ');

            }


            std::cout << std::endl;

        }
    }

    std::cout << std::endl;
    if(clippedFinalElements)
    {
        std::cout << "(deeper levels omitted due to space limitations)" << std::endl;
    }


    if(!std::is_same<Key, uint8_t>::value)
    {
        std::cout << "Tree Placeholders:------------------" << std::endl;
        for(typename std::map<Key, uint8_t>::iterator placeholdersIter = valuePlaceholders.begin(); placeholdersIter != valuePlaceholders.end(); ++placeholdersIter)
        {
            std::cout << '[' << std::setfill('0') << std::setw(2) << ((uint16_t)placeholdersIter->second) << "] -> ";
            std::cout.flags(origCoutState);
            std::cout << '(' << placeholdersIter->first << ", ";

            typename BinarySearchTree<Key, Value>::iterator elementIter = this->find(placeholdersIter->first);
            if(elementIter == this->end())
            {
                std::cout << "<error: lookup failed>";
            }
            else
            {
                std::cout << elementIter->second;
            }

            std::cout << ')' << std::endl;


        }
    }
    std::cout.flags(origCoutState);

}

#endif

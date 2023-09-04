// compute_keypoints_octree.cpp
// 2023 AUG 22
// Tershire

// referred: 

// test ORB-SLAM3's keypoint computing mechanism

// command: 


#include "orb_extractor.h"

// prototype
static bool compare_nodes(std::pair<int, Extractor_Node*>& e1, std::pair<int, Extractor_Node*>& e2);

// GLOBAL SETTING /////////////////////////////////////////////////////////////
const int EDGE_THRESHOLD = 19; // ORB-SLAM3 setting
const int PATCH_SIZE = 31;

// public XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// constructor & destructor ///////////////////////////////////////////////////
ORB_Extractor::ORB_Extractor(const int& num_features, const float& scale_factor, 
    const int& num_levels, const int& first_FAST_threshold, const int& second_FAST_threshold)
    : num_features_(num_features), scale_factor_(scale_factor), num_levels_(num_levels),
    first_FAST_threshold_(first_FAST_threshold), second_FAST_threshold_(second_FAST_threshold)
    {
        scale_factors_for_each_level_.resize(num_levels);
        sigma2_levels_for_each_level_.resize(num_levels);
        scale_factors_for_each_level_[0] = 1.0F;
        sigma2_levels_for_each_level_[0] = 1.0F;
        for (int i = 1; i < num_levels; ++i)
        {
            scale_factors_for_each_level_[i] = scale_factors_for_each_level_[i - 1] * scale_factor;
            sigma2_levels_for_each_level_[i] = scale_factors_for_each_level_[i] * scale_factors_for_each_level_[i];
        }

        inverse_scale_factors_for_each_level_.resize(num_levels);
        inverse_sigma2_levels_for_each_level_.resize(num_levels);
        for (int i = 0; i < num_levels; ++i)
        {
            inverse_scale_factors_for_each_level_[i] = 1.0F / scale_factors_for_each_level_[i];
            inverse_sigma2_levels_for_each_level_[i] = 1.0F / sigma2_levels_for_each_level_[i];
        }

        image_pyramid_.resize(num_levels);

        num_features_at_each_level_.resize(num_levels);
        float factor = 1.0F / scale_factor;
        float num_desired_features_per_scale = num_features * (1 - factor) / (1 - (float) pow((double) factor, (double) num_levels));

        int num_total_features_in_pyramid = 0;
        for (int level = 0; level < num_levels - 1; ++level)
        {
            num_features_at_each_level_[level] = cvRound(num_desired_features_per_scale);
            num_total_features_in_pyramid += num_features_at_each_level_[level];
            num_desired_features_per_scale *= factor;
        }
        num_features_at_each_level_[num_levels - 1] = std::max(num_features - num_total_features_in_pyramid, 0);

        // const int num_points = 512;
        // const cv::Point* pattern0 = (const cv::Point*) bit_pattern_31_;
        // std::copy(pattern0, pattern0 + num_points, std::back_inserter(pattern));

        // orientation
        // pre-compute the end of a row in a circular patch
        // umax.resize(HALF_PATCH_SIZE + 1);

        // int v, v0, vmax = cvFloor(HALF_PATCH_SIZE * sqrt(2.f) / 2 + 1);
        // int vmin = cvCeil(HALF_PATCH_SIZE * sqrt(2.f) / 2);
        // const double hp2 = HALF_PATCH_SIZE*HALF_PATCH_SIZE;
        // for (v = 0; v <= vmax; ++v)
        //     umax[v] = cvRound(sqrt(hp2 - v * v));

        // Make sure we are symmetric
        // for (v = HALF_PATCH_SIZE, v0 = 0; v >= vmin; --v)
        // {
        //     while (umax[v0] == umax[v0 + 1])
        //         ++v0;
        //     umax[v] = v0;
        //     ++v0;
        // }
    }

// ----------------------------------------------------------------------------
void Extractor_Node::divide_node(Extractor_Node& node1, Extractor_Node& node2, Extractor_Node& node3, Extractor_Node& node4)
{
    const int HALF_DELTA_X = ceil(static_cast<float>(UR.x - UL.x) / 2);
    const int HALF_DELTA_Y = ceil(static_cast<float>(BR.y - UL.y) / 2);

    //Define boundaries of childs
    node1.UL = UL;
    node1.UR = cv::Point2i(UL.x + HALF_DELTA_X, UL.y);
    node1.BL = cv::Point2i(UL.x, UL.y + HALF_DELTA_Y);
    node1.BR = cv::Point2i(UL.x + HALF_DELTA_X, UL.y + HALF_DELTA_Y);
    node1.keypoints.reserve(keypoints.size());

    node2.UL = node1.UR;
    node2.UR = UR;
    node2.BL = node1.BR;
    node2.BR = cv::Point2i(UR.x, UL.y + HALF_DELTA_Y);
    node2.keypoints.reserve(keypoints.size());

    node3.UL = node1.BL;
    node3.UR = node1.BR;
    node3.BL = BL;
    node3.BR = cv::Point2i(node1.BR.x, BL.y);
    node3.keypoints.reserve(keypoints.size());

    node4.UL = node3.UR;
    node4.UR = node2.BR;
    node4.BL = node3.BR;
    node4.BR = BR;
    node4.keypoints.reserve(keypoints.size());

    // associate points to childs
    for (size_t i = 0; i < keypoints.size(); ++i)
    {
        const cv::KeyPoint& keypoint = keypoints[i];
        if (keypoint.pt.x < node1.UR.x)
        {
            if (keypoint.pt.y < node1.BR.y)
                node1.keypoints.push_back(keypoint);
            else
                node3.keypoints.push_back(keypoint);
        }
        else if (keypoint.pt.y < node1.BR.y)
            node2.keypoints.push_back(keypoint);
        else
            node4.keypoints.push_back(keypoint);
    }

    if (node1.keypoints.size() == 1)
        node1.is_one_keypoint_left = true;
    if (node2.keypoints.size() == 1)
        node2.is_one_keypoint_left = true;
    if (node3.keypoints.size() == 1)
        node3.is_one_keypoint_left = true;
    if (node4.keypoints.size() == 1)
        node4.is_one_keypoint_left = true;
}


// ----------------------------------------------------------------------------
static bool compare_nodes(std::pair<int, Extractor_Node*>& e1, std::pair<int, Extractor_Node*>& e2)
{
    if (e1.first < e2.first)
    {
        return true;
    }
    else if (e1.first > e2.first)
    {
        return false;
    }
    else
    {
        if(e1.second->UL.x < e2.second->UL.x)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

// ----------------------------------------------------------------------------
std::vector<cv::KeyPoint> ORB_Extractor::distribute_keypoints_octree(const std::vector<cv::KeyPoint>& keypoints_to_distribute, 
    const int& min_border_x, const int& max_border_x, const int& min_border_y, const int& max_border_y, 
    const int& num_features, const int& level)
{
    // compute how many initial nodes
    const int NUM_INITIAL_NODES = round(static_cast<float>(max_border_x - min_border_x) / (max_border_y - min_border_y));

    const float hX = static_cast<float>(max_border_x - min_border_x) / NUM_INITIAL_NODES;

    std::list<Extractor_Node> nodes;

    std::vector<Extractor_Node*> initial_nodes;
    initial_nodes.resize(NUM_INITIAL_NODES);

    for (int i = 0; i < NUM_INITIAL_NODES; ++i)
    {
        Extractor_Node node;
        node.UL = cv::Point2i(hX * static_cast<float>(i)    , 0);
        node.UR = cv::Point2i(hX * static_cast<float>(i + 1), 0);
        node.BL = cv::Point2i(node.UL.x, max_border_y - min_border_y);
        node.BR = cv::Point2i(node.UR.x, max_border_y - min_border_y);
        node.keypoints.reserve(keypoints_to_distribute.size());

        nodes.push_back(node);
        initial_nodes[i] = &nodes.back();
    }

    // associate points to childs
    for (size_t i = 0; i < keypoints_to_distribute.size(); ++i)
    {
        const cv::KeyPoint& keypoint = keypoints_to_distribute[i];
        initial_nodes[keypoint.pt.x / hX]->keypoints.push_back(keypoint);
    }

    std::list<Extractor_Node>::iterator it = nodes.begin();

    while (it != nodes.end())
    {
        if (it->keypoints.size() == 1)
        {
            it->is_one_keypoint_left = true;
            it++;
        }
        else if (it->keypoints.empty())
            it = nodes.erase(it);
        else
            it++;
    }

    bool is_finished = false;
    int iteration = 0;
    std::vector<std::pair<int, Extractor_Node*>> node_size_and_pointers;
    node_size_and_pointers.reserve(nodes.size() * 4);

    while (!is_finished)
    {
        iteration++;

        int previous_node_size = nodes.size();

        it = nodes.begin();

        int num_expansions = 0;

        node_size_and_pointers.clear();

        while (it != nodes.end())
        {
            if(it->is_one_keypoint_left)
            {
                // if node only contains one point do not subdivide and continue
                it++;
                continue;
            }
            else
            {
                // if more than one point, subdivide
                Extractor_Node node1, node2, node3, node4;
                it->divide_node(node1, node2, node3, node4);

                // add childs if they contain points
                if (node1.keypoints.size() > 0)
                {
                    nodes.push_front(node1);
                    if (node1.keypoints.size() > 1)
                    {
                        num_expansions++;
                        node_size_and_pointers.push_back(std::make_pair(node1.keypoints.size(), &nodes.front()));
                        nodes.front().it = nodes.begin();
                    }
                }
                if (node2.keypoints.size() > 0)
                {
                    nodes.push_front(node2);
                    if (node2.keypoints.size() > 1)
                    {
                        num_expansions++;
                        node_size_and_pointers.push_back(std::make_pair(node2.keypoints.size(), &nodes.front()));
                        nodes.front().it = nodes.begin();
                    }
                }
                if (node3.keypoints.size() > 0)
                {
                    nodes.push_front(node3);
                    if (node3.keypoints.size() > 1)
                    {
                        num_expansions++;
                        node_size_and_pointers.push_back(std::make_pair(node3.keypoints.size(), &nodes.front()));
                        nodes.front().it = nodes.begin();
                    }
                }
                if (node4.keypoints.size() > 0)
                {
                    nodes.push_front(node4);
                    if (node4.keypoints.size() > 1)
                    {
                        num_expansions++;
                        node_size_and_pointers.push_back(std::make_pair(node4.keypoints.size(), &nodes.front()));
                        nodes.front().it = nodes.begin();
                    }
                }

                it = nodes.erase(it);
                continue;
            }
        }

        // finish if there are more nodes than required features
        // or all nodes contain just one point
        if ((int) nodes.size() >= num_features || (int) nodes.size() == previous_node_size)
        {
            is_finished = true;
        }
        else if(((int) nodes.size() + num_expansions * 3) > num_features)
        {
            while (!is_finished)
            {
                previous_node_size = nodes.size();

                std::vector<std::pair<int,Extractor_Node*>> previous_node_sizes_and_pointers = node_size_and_pointers;
                node_size_and_pointers.clear();

                sort(previous_node_sizes_and_pointers.begin(), previous_node_sizes_and_pointers.end(), compare_nodes);
                for (int j = previous_node_sizes_and_pointers.size() - 1; j >= 0; --j)
                {
                    Extractor_Node node1, node2, node3, node4;
                    previous_node_sizes_and_pointers[j].second->divide_node(node1, node2, node3, node4);

                    // add childs if they contain points
                    if (node1.keypoints.size() > 0)
                    {
                        nodes.push_front(node1);
                        if (node1.keypoints.size() > 1)
                        {
                            node_size_and_pointers.push_back(std::make_pair(node1.keypoints.size(), &nodes.front()));
                            nodes.front().it = nodes.begin();
                        }
                    }
                    if (node2.keypoints.size() > 0)
                    {
                        nodes.push_front(node2);
                        if(node2.keypoints.size() > 1)
                        {
                            node_size_and_pointers.push_back(std::make_pair(node2.keypoints.size(), &nodes.front()));
                            nodes.front().it = nodes.begin();
                        }
                    }
                    if (node3.keypoints.size() > 0)
                    {
                        nodes.push_front(node3);
                        if (node3.keypoints.size() > 1)
                        {
                            node_size_and_pointers.push_back(std::make_pair(node3.keypoints.size(), &nodes.front()));
                            nodes.front().it = nodes.begin();
                        }
                    }
                    if (node4.keypoints.size() > 0)
                    {
                        nodes.push_front(node4);
                        if (node4.keypoints.size() > 1)
                        {
                            node_size_and_pointers.push_back(std::make_pair(node4.keypoints.size(), &nodes.front()));
                            nodes.front().it = nodes.begin();
                        }
                    }

                    nodes.erase(previous_node_sizes_and_pointers[j].second->it);

                    if((int) nodes.size() >= num_features)
                        break;
                }

                if((int) nodes.size() >= num_features || (int) nodes.size() == previous_node_size)
                    is_finished = true;

            }
        }
    }

    // retain the best point in each node
    std::vector<cv::KeyPoint> distributed_keypoints;
    distributed_keypoints.reserve(num_features);
    for (std::list<Extractor_Node>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        std::vector<cv::KeyPoint>& node_keypoints = it->keypoints;
        cv::KeyPoint* keypoint = &node_keypoints[0];
        float max_response = keypoint->response;

        for(size_t i = 1; i < node_keypoints.size(); ++i)
        {
            if(node_keypoints[i].response>max_response)
            {
                keypoint = &node_keypoints[i];
                max_response = node_keypoints[i].response;
            }
        }

        distributed_keypoints.push_back(*keypoint);
    }

    return distributed_keypoints;
}

// ----------------------------------------------------------------------------
std::vector<cv::KeyPoint> ORB_Extractor::distribute_keypoints_octree(const std::vector<cv::KeyPoint>& keypoints_to_distribute, 
    const int& num_features, const int& level)
{
    // compute border
    int min_border_x = EDGE_THRESHOLD - 3;
    int min_border_y = min_border_x;
    int max_border_x = image_pyramid_[level].cols - EDGE_THRESHOLD + 3;
    int max_border_y = image_pyramid_[level].rows - EDGE_THRESHOLD + 3;

    // compute how many initial nodes
    const int NUM_INITIAL_NODES = round(static_cast<float>(max_border_x - min_border_x) / (max_border_y - min_border_y));

    const float hX = static_cast<float>(max_border_x - min_border_x) / NUM_INITIAL_NODES;

    std::list<Extractor_Node> nodes;

    std::vector<Extractor_Node*> initial_nodes;
    initial_nodes.resize(NUM_INITIAL_NODES);

    for (int i = 0; i < NUM_INITIAL_NODES; ++i)
    {
        Extractor_Node node;
        node.UL = cv::Point2i(hX * static_cast<float>(i)    , 0);
        node.UR = cv::Point2i(hX * static_cast<float>(i + 1), 0);
        node.BL = cv::Point2i(node.UL.x, max_border_y - min_border_y);
        node.BR = cv::Point2i(node.UR.x, max_border_y - min_border_y);
        node.keypoints.reserve(keypoints_to_distribute.size());

        nodes.push_back(node);
        initial_nodes[i] = &nodes.back();
    }

    // associate points to childs
    for (size_t i = 0; i < keypoints_to_distribute.size(); ++i)
    {
        const cv::KeyPoint& keypoint = keypoints_to_distribute[i];
        initial_nodes[keypoint.pt.x / hX]->keypoints.push_back(keypoint);
    }

    std::list<Extractor_Node>::iterator it = nodes.begin();

    while (it != nodes.end())
    {
        if (it->keypoints.size() == 1)
        {
            it->is_one_keypoint_left = true;
            it++;
        }
        else if (it->keypoints.empty())
            it = nodes.erase(it);
        else
            it++;
    }

    bool is_finished = false;
    int iteration = 0;
    std::vector<std::pair<int, Extractor_Node*>> node_size_and_pointers;
    node_size_and_pointers.reserve(nodes.size() * 4);

    while (!is_finished)
    {
        iteration++;

        int previous_node_size = nodes.size();

        it = nodes.begin();

        int num_expansions = 0;

        node_size_and_pointers.clear();

        while (it != nodes.end())
        {
            if(it->is_one_keypoint_left)
            {
                // if node only contains one point do not subdivide and continue
                it++;
                continue;
            }
            else
            {
                // if more than one point, subdivide
                Extractor_Node node1, node2, node3, node4;
                it->divide_node(node1, node2, node3, node4);

                // add childs if they contain points
                if (node1.keypoints.size() > 0)
                {
                    nodes.push_front(node1);
                    if (node1.keypoints.size() > 1)
                    {
                        num_expansions++;
                        node_size_and_pointers.push_back(std::make_pair(node1.keypoints.size(), &nodes.front()));
                        nodes.front().it = nodes.begin();
                    }
                }
                if (node2.keypoints.size() > 0)
                {
                    nodes.push_front(node2);
                    if (node2.keypoints.size() > 1)
                    {
                        num_expansions++;
                        node_size_and_pointers.push_back(std::make_pair(node2.keypoints.size(), &nodes.front()));
                        nodes.front().it = nodes.begin();
                    }
                }
                if (node3.keypoints.size() > 0)
                {
                    nodes.push_front(node3);
                    if (node3.keypoints.size() > 1)
                    {
                        num_expansions++;
                        node_size_and_pointers.push_back(std::make_pair(node3.keypoints.size(), &nodes.front()));
                        nodes.front().it = nodes.begin();
                    }
                }
                if (node4.keypoints.size() > 0)
                {
                    nodes.push_front(node4);
                    if (node4.keypoints.size() > 1)
                    {
                        num_expansions++;
                        node_size_and_pointers.push_back(std::make_pair(node4.keypoints.size(), &nodes.front()));
                        nodes.front().it = nodes.begin();
                    }
                }

                it = nodes.erase(it);
                continue;
            }
        }

        // finish if there are more nodes than required features
        // or all nodes contain just one point
        if ((int) nodes.size() >= num_features || (int) nodes.size() == previous_node_size)
        {
            is_finished = true;
        }
        else if(((int) nodes.size() + num_expansions * 3) > num_features)
        {
            while (!is_finished)
            {
                previous_node_size = nodes.size();

                std::vector<std::pair<int,Extractor_Node*>> previous_node_sizes_and_pointers = node_size_and_pointers;
                node_size_and_pointers.clear();

                sort(previous_node_sizes_and_pointers.begin(), previous_node_sizes_and_pointers.end(), compare_nodes);
                for (int j = previous_node_sizes_and_pointers.size() - 1; j >= 0; --j)
                {
                    Extractor_Node node1, node2, node3, node4;
                    previous_node_sizes_and_pointers[j].second->divide_node(node1, node2, node3, node4);

                    // add childs if they contain points
                    if (node1.keypoints.size() > 0)
                    {
                        nodes.push_front(node1);
                        if (node1.keypoints.size() > 1)
                        {
                            node_size_and_pointers.push_back(std::make_pair(node1.keypoints.size(), &nodes.front()));
                            nodes.front().it = nodes.begin();
                        }
                    }
                    if (node2.keypoints.size() > 0)
                    {
                        nodes.push_front(node2);
                        if(node2.keypoints.size() > 1)
                        {
                            node_size_and_pointers.push_back(std::make_pair(node2.keypoints.size(), &nodes.front()));
                            nodes.front().it = nodes.begin();
                        }
                    }
                    if (node3.keypoints.size() > 0)
                    {
                        nodes.push_front(node3);
                        if (node3.keypoints.size() > 1)
                        {
                            node_size_and_pointers.push_back(std::make_pair(node3.keypoints.size(), &nodes.front()));
                            nodes.front().it = nodes.begin();
                        }
                    }
                    if (node4.keypoints.size() > 0)
                    {
                        nodes.push_front(node4);
                        if (node4.keypoints.size() > 1)
                        {
                            node_size_and_pointers.push_back(std::make_pair(node4.keypoints.size(), &nodes.front()));
                            nodes.front().it = nodes.begin();
                        }
                    }

                    nodes.erase(previous_node_sizes_and_pointers[j].second->it);

                    if((int) nodes.size() >= num_features)
                        break;
                }

                if((int) nodes.size() >= num_features || (int) nodes.size() == previous_node_size)
                    is_finished = true;

            }
        }
    }

    // retain the best point in each node
    std::vector<cv::KeyPoint> distributed_keypoints;
    distributed_keypoints.reserve(num_features);
    for (std::list<Extractor_Node>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        std::vector<cv::KeyPoint>& node_keypoints = it->keypoints;
        cv::KeyPoint* keypoint = &node_keypoints[0];
        float max_response = keypoint->response;

        for(size_t i = 1; i < node_keypoints.size(); ++i)
        {
            if(node_keypoints[i].response>max_response)
            {
                keypoint = &node_keypoints[i];
                max_response = node_keypoints[i].response;
            }
        }

        distributed_keypoints.push_back(*keypoint);
    }

    return distributed_keypoints;
}

// ----------------------------------------------------------------------------
void ORB_Extractor::compute_keypoints_octree(std::vector<std::vector<cv::KeyPoint>>& keypoints_at_each_level)
{
    keypoints_at_each_level.resize(num_levels_);

    const float DIVISION_FACTOR = 35;

    for (int level = 0; level < num_levels_; ++level)
    {
        const int MIN_BORDER_X = EDGE_THRESHOLD - 3;
        const int MIN_BORDER_Y = MIN_BORDER_X;
        const int MAX_BORDER_X = image_pyramid_[level].cols - EDGE_THRESHOLD + 3;
        const int MAX_BORDER_Y = image_pyramid_[level].rows - EDGE_THRESHOLD + 3;

        std::vector<cv::KeyPoint> keypoints_to_distribute;
        keypoints_to_distribute.reserve(num_features_ * 10);

        const float WIDTH  = (MAX_BORDER_X - MIN_BORDER_X);
        const float HEIGHT = (MAX_BORDER_Y - MIN_BORDER_Y);

        const int NUM_COLS = WIDTH /DIVISION_FACTOR; // number of columns of divided tiles
        const int NUM_ROWS = HEIGHT/DIVISION_FACTOR; // number of rows    of divided tiles
        const int TILE_WIDTH  = ceil(WIDTH /NUM_COLS);
        const int TILE_HEIGHT = ceil(HEIGHT/NUM_ROWS);

        for (int i = 0; i < NUM_ROWS; ++i)
        {   
            // get one tile
            float y_min = MIN_BORDER_Y + i*TILE_HEIGHT;
            float y_max = y_min + TILE_HEIGHT + 6;

            if (y_min >= MAX_BORDER_Y - 3)
                continue;
                
            if (y_max > MAX_BORDER_Y)
                y_max = MAX_BORDER_Y;

            for (int j = 0; j < NUM_COLS; ++j)
            {
                float x_min =MIN_BORDER_X + j*TILE_WIDTH;
                float x_max = x_min + TILE_WIDTH + 6;

                if (x_min >= MAX_BORDER_X - 6)
                    continue;

                if (x_max > MAX_BORDER_X)
                    x_max = MAX_BORDER_X;

                // 1st FAST try
                std::vector<cv::KeyPoint> keypoints_in_tile;
                FAST(image_pyramid_[level].rowRange(y_min, y_max).colRange(x_min, x_max),
                    keypoints_in_tile, first_FAST_threshold_, true);

                // 2nd FAST try
                if(keypoints_in_tile.empty())
                {
                    FAST(image_pyramid_[level].rowRange(y_min, y_max).colRange(x_min, x_max),
                        keypoints_in_tile, second_FAST_threshold_, true);
                }
                
                if(!keypoints_in_tile.empty())
                {
                    for(std::vector<cv::KeyPoint>::iterator it = keypoints_in_tile.begin(); it != keypoints_in_tile.end(); ++it)
                    {
                        (*it).pt.x += j*TILE_WIDTH;
                        (*it).pt.y += i*TILE_HEIGHT;
                        keypoints_to_distribute.push_back(*it);
                    }
                }
            }
        }

        std::vector<cv::KeyPoint>& keypoints = keypoints_at_each_level[level];
        keypoints.reserve(num_features_);

        keypoints = distribute_keypoints_octree(keypoints_to_distribute, 
            MIN_BORDER_X, MAX_BORDER_X, MIN_BORDER_Y, MAX_BORDER_Y, 
            num_features_at_each_level_[level], level);

        // (TO DO) consider if the info. below is really needed
        int scaled_patch_size = PATCH_SIZE * scale_factors_for_each_level_[level];

        // add border to coordinates and scale information
        const int nkps = keypoints.size();
        for (int i = 0; i <nkps; ++i)
        {
            keypoints[i].pt.x += MIN_BORDER_X;
            keypoints[i].pt.y += MIN_BORDER_Y;
            keypoints[i].octave = level;
            keypoints[i].size = scaled_patch_size;
        }
    }

    // // compute orientations
    // for (int level = 0; level < num_levels_; ++level)
    //     compute_orientation(image_pyramid_[level], keypoints_at_each_level[level], umax);
}

// ----------------------------------------------------------------------------
void ORB_Extractor::compute_pyramid(cv::Mat image)
{
    for (int level = 0; level < num_levels_; ++level)
    {
        float scale = inverse_scale_factors_for_each_level_[level];
        cv::Size scaled_image_size(cvRound((float) image.cols * scale), cvRound((float) image.rows * scale));
        cv::Size augmented_scaled_image_size(scaled_image_size.width + EDGE_THRESHOLD*2, scaled_image_size.height + EDGE_THRESHOLD*2);
        cv::Mat temp(augmented_scaled_image_size, image.type());
        image_pyramid_[level] = temp(cv::Rect(EDGE_THRESHOLD, EDGE_THRESHOLD, scaled_image_size.width, scaled_image_size.height));

        // compute the resized image
        if (level != 0)
        {
            cv::resize(image_pyramid_[level - 1], image_pyramid_[level], scaled_image_size, 0, 0, cv::INTER_LINEAR);

            cv::copyMakeBorder(image_pyramid_[level], temp, EDGE_THRESHOLD, EDGE_THRESHOLD, EDGE_THRESHOLD, EDGE_THRESHOLD,
                cv::BORDER_REFLECT_101 + cv::BORDER_ISOLATED);
        }
        else
        {
            cv::copyMakeBorder(image, temp, EDGE_THRESHOLD, EDGE_THRESHOLD, EDGE_THRESHOLD, EDGE_THRESHOLD,
                cv::BORDER_REFLECT_101);
        }
    }
}

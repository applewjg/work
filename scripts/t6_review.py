#!/usr/bin/env python
#coding: utf-8
"""
T6 工程组排序脚本
"""
import random
import logging


# 评委排序权重
REVIEWER_RANK_WEIGHT = 1.0
# 部门排序权重
TEAM_RANK_WEIGHT = 1.0
# 分数排序权重
SCORE_RANK_WEIGHT = 0.2


def load_applicants():
    """加载申请人名单, app_list为原始序列表，app_rank是名字到最终排序的映射"""
    app_list = []
    app_rank = dict()
    rank = 1
    for name in open('applicants.txt'):
        app_list.append(name.decode('utf8').strip())
        app_rank[name.decode('utf8').strip()] = rank
        rank += 1
    return app_list, app_rank

    
def check_relation_data(segments, app_list):
    """
    校验偏序关系数据行的合法性
    """
    for i in xrange(1, len(segments), 2):
        if segments[i] not in app_list:
            logging.fatal("Invalid person: %s", segments[i])
            raise Exception
    for i in xrange(2, len(segments), 2):
        if segments[i] not in ('>', '='):
            logging.fatal("Invalid relation operator: %s", segments[i])
            raise Exception
            
            
def load_ranks(path, app_list):
    """
    加载偏序关系
    named_pairs记录每个人的偏序关系，用于最终统计分析
    aggregated_pairs对偏序关系进行汇聚，用于加速计算
    """
    named_pairs = []
    aggregated_pairs = dict()
    for line in open(path):
        segments = line.decode('utf8').strip().split()
        if len(segments) < 4:
            continue
        check_relation_data(segments, app_list)        
        # 根据 > 划分layer，layer内部用 = 号连接。偏序关系仅发生在层间
        layers = []
        current_layer = [segments[1]]
        for i in xrange(2, len(segments), 2):
            if segments[i] == '>':
                layers.append(current_layer)
                current_layer = []
            current_layer.append(segments[i + 1])
        if current_layer:
            layers.append(current_layer)
        # 层间生成所有偏序关系对
        local_pairs = set()
        for i in xrange(len(layers) - 1):
            for j in xrange(i + 1, len(layers)):
                for k in layers[i]:
                    for l in layers[j]:
                        pair = (k, l)
                        if pair in aggregated_pairs:
                            aggregated_pairs[pair] += 1
                        else:
                            aggregated_pairs[pair] = 1
                        local_pairs.add(pair)
        named_pairs.append((segments[0], local_pairs))
    return aggregated_pairs, named_pairs


def calc_rank_score(app_rank, reviewer_ranks, team_ranks, score_rank):
    """
    为排序方案app_rank打分
    遍历app_rank中的所有有序对，如果和评委、团队、基础打分排序一致则加分；
    不一致则扣分；找不到则不加不减
    """
    score = 0
    for a, a_rank in app_rank.iteritems():
        for b, b_rank in app_rank.iteritems():
            if a_rank >= b_rank:
                continue
            pair = (a, b)
            inv_pair = (b, a)
            if pair in reviewer_ranks:
                score += reviewer_ranks[pair]
            if inv_pair in reviewer_ranks:
                score -= reviewer_ranks[inv_pair]
            if pair in team_ranks:
                score += team_ranks[pair]
            if inv_pair in team_ranks:
                score -= team_ranks[inv_pair]
            if pair in score_rank:
                score += score_rank[pair]
            if inv_pair in score_rank:
                score -= score_rank[inv_pair]
    return score


def show_reverse_order_stat(app_rank, named_ranks):
    """分析显示最终排序下，各评委的逆序统计。展示评委和最终排序的一致程度"""
    for name, pairs in named_ranks:
        reverse_list = []
        for p in pairs:
            if app_rank[p[0]] > app_rank[p[1]]:
                reverse_list.append(p)
        print name, len(reverse_list),
        for p in reverse_list:
            print " (%s>%s)" % (p[0], p[1]),
        print


def show_position_gap(app_rank, reviewer_ranks, team_ranks, score_rank, 
                      named_reviewer_ranks, named_team_ranks, named_score_rank):
    """
    分析展示最终排序相临pair之间的差异，包括正向支持和反向支持的分数和原因
    用于分析局部排序的靠谱程度、区分度
    """
    ordered_app_list = sorted([n for n in app_rank], key=lambda n: app_rank[n])
    for i in xrange(len(ordered_app_list) - 1):
        a = ordered_app_list[i]
        b = ordered_app_list[i + 1]
        pairs = [(a, b), (b, a)]    # 正、逆序对
        scores = [0, 0]             # 正、逆序支持分数
        names = [[], []]            # 正、逆序支持的人
        for pi in xrange(2):
            if pairs[pi] in reviewer_ranks:
                scores[pi] += reviewer_ranks[pairs[pi]]
            if pairs[pi] in team_ranks:
                scores[pi] += team_ranks[pairs[pi]]
            if pairs[pi] in score_rank:
                scores[pi] += score_rank[pairs[pi]]
            names[pi].extend([k for k, v in named_reviewer_ranks if pairs[pi] in
v])
            names[pi].extend([k for k, v in named_team_ranks if pairs[pi] in v])
            names[pi].extend([k for k, v in named_score_rank if pairs[pi] in v])
        print a, scores[0], '[%s]' % ','.join(names[0]), scores[1], '[%s]' % ','
.join(names[1])


def optimize_rank(app_list, app_rank, reviewer_ranks, team_ranks, score_rank):
    """穷举+随机shuffle优化排序"""
    score = calc_rank_score(app_rank, reviewer_ranks, team_ranks, score_rank)
    rand_walk_round = 0
    MAX_RAND_WALK_ROUND = 50000
    while rand_walk_round < MAX_RAND_WALK_ROUND:
        # 穷举所有两两调换，贪心法优化排序，直到任意两两调换都不会变好
        rank_changed = True
        while rank_changed:
            rank_changed = False
            for a, a_rank in app_rank.iteritems():
                for b, b_rank in app_rank.iteritems():
                    if a_rank >= b_rank:
                        continue
                    app_rank[a], app_rank[b] = app_rank[b], app_rank[a] # swap
                    new_score = calc_rank_score(app_rank, reviewer_ranks, team_r
anks, score_rank)
                    if (new_score > score):
                        score = new_score
                        print score, "swap(%s, %s)" % (a, b)
                        rank_changed = True
                    else:
                        app_rank[a], app_rank[b] = app_rank[b], app_rank[a] # re
store
        # 尝试多item随机shuffle，跳出局部最优
        while rand_walk_round < MAX_RAND_WALK_ROUND:
            rand_walk_round += 1
            # random sample
            shuffle_item_count = random.randint(3, 5)
            names = random.sample(app_list, shuffle_item_count)
            # random shuffle rank
            ranks = [app_rank[n] for n in names]
            new_ranks = ranks[:]
            random.shuffle(new_ranks)
            for i in xrange(shuffle_item_count):
                app_rank[names[i]] = new_ranks[i]
            new_score = calc_rank_score(app_rank, reviewer_ranks, team_ranks, sc
ore_rank)
            if (new_score > score):
                score = new_score
                print score, "random shuffle ", shuffle_item_count
                break
            else:
                # restore rank
                for i in xrange(shuffle_item_count):
                    app_rank[names[i]] = ranks[i]        
        

def main():
    """T6工程组内部排序算法"""
    # 加载申请人
    app_list, app_rank = load_applicants()
    # 加载三种偏序关系
    reviewer_ranks, named_reviewer_ranks = load_ranks('reviewer_ranks.txt', app_
list)
    for p in reviewer_ranks:
        reviewer_ranks[p] *= REVIEWER_RANK_WEIGHT
    team_ranks, named_team_ranks = load_ranks('team_ranks.txt', app_list)
    for p in team_ranks:
        team_ranks[p] *= TEAM_RANK_WEIGHT
    score_rank, named_score_rank = load_ranks('score_rank.txt', app_list)
    for p in score_rank:
        score_rank[p] *= SCORE_RANK_WEIGHT
    
    # 迭代求逆序率最小的排序
    optimize_rank(app_list, app_rank, reviewer_ranks, team_ranks, score_rank)
    
    # 输出结果
    print "############### final rank ##################"
    for a in app_list:
        print a, app_rank[a]
    print "############### position gap ##################"
    show_position_gap(app_rank, reviewer_ranks, team_ranks, score_rank, 
                      named_reviewer_ranks, named_team_ranks, named_score_rank)
    print "############### reviewer reverse order ##################"
    show_reverse_order_stat(app_rank, named_reviewer_ranks)
    print "############### team reverse order ##################"
    show_reverse_order_stat(app_rank, named_team_ranks)
    print "############### score reverse order ##################"
    show_reverse_order_stat(app_rank, named_score_rank)


if __name__ == '__main__':
    main()

class Twitter {
public:
	/** Initialize your data structure here. */
	Twitter() {}

	/** Compose a new tweet. */
	void postTweet(int userId, int tweetId) {
		user_tweets_[userId].push_back({ curr_time_++, tweetId });
	}

	/** Retrieve the 10 most recent tweet ids in the user's news feed. Each item in the news feed must be posted by users who the user followed or by the user herself. Tweets must be ordered from most recent to least recent. */
	vector<int> getNewsFeed(int userId) {
		priority_queue<SortData> feed;

		AddTweetDataToPQ(feed, userId);

		for (auto followee_id : user_follows_[userId])
			AddTweetDataToPQ(feed, followee_id);

		vector<int> res;

		while (res.size() < 10 && !feed.empty()) {
			auto data = feed.top();
			feed.pop();
			res.push_back(data.tweet_id);
			AddTweetDataToPQ(feed, data.user_id, data.pos_in_user_tweets);
		}

		return res;
	}

	/** Follower follows a followee. If the operation is invalid, it should be a no-op. */
	void follow(int followerId, int followeeId) {
		if (!user_tweets_.count(followerId)) return;
		user_follows_[followerId].insert(followeeId);
	}

	/** Follower unfollows a followee. If the operation is invalid, it should be a no-op. */
	void unfollow(int followerId, int followeeId) {
		if (!user_tweets_.count(followerId) || !user_tweets_.count(followeeId))
			return;
		user_follows_[followerId].erase(followeeId);
	}
private:
	int curr_time_{ 0 };

	struct Tweet {
		int timestamp;
		int id;
	};

	unordered_map<int, vector<Tweet>> user_tweets_;
	unordered_map<int, set<int>> user_follows_;

	struct SortData {
		int timestamp;
		int tweet_id;
		int user_id;
		int pos_in_user_tweets;

		bool operator < (const SortData& other) const {
			return timestamp < other.timestamp;
		}
	};

	void AddTweetDataToPQ(priority_queue<SortData>& feed, int userId, int last_tweet_pos = -1) {
		if (!user_tweets_.count(userId) || user_tweets_[userId].empty() || last_tweet_pos == 0)
			return;

		if (last_tweet_pos == -1)
			last_tweet_pos = user_tweets_[userId].size() - 1;

		auto& last_tweet = user_tweets_[userId][last_tweet_pos];
		feed.push({ last_tweet.timestamp, last_tweet.id, userId, last_tweet_pos });
	}
};

int main() {
	Twitter t;
	t.postTweet(1, 5);
	auto f1 = t.getNewsFeed(1);
	t.follow(1, 2);
	t.postTweet(2, 6);
	auto f2 = t.getNewsFeed(1);
	t.unfollow(1, 2);
	auto f3 = t.getNewsFeed(1);

	cin.get();
	return 0;
}

//["Twitter", "postTweet", "getNewsFeed", "follow", "postTweet", "getNewsFeed", "unfollow", "getNewsFeed"]
//[[], [1, 5], [1], [1, 2], [2, 6], [1], [1, 2], [1]]

/**
 * Your Twitter object will be instantiated and called as such:
 * Twitter obj = new Twitter();
 * obj.postTweet(userId,tweetId);
 * vector<int> param_2 = obj.getNewsFeed(userId);
 * obj.follow(followerId,followeeId);
 * obj.unfollow(followerId,followeeId);
 */
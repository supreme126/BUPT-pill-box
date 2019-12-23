// pages/clock2/clock2.js
Page({
  data: {
    key5: 'clock_sum',

    test: "test",

    clock_number_key: [{
      key: 'clock1',
    },
    {
      key: 'clock2'
    },
    {
      key: 'clock3'
    },
    {
      key: 'clock4'
    },
    {
      key: 'clock5'
    },
    {
      key: 'clock6'
    },
    {
      key: 'clock7'
    }
    ],

    switch_data: [{
      bool: true
    },
    {
      bool: true
    },
    {
      bool: true
    },
    {
      bool: true
    },
    {
      bool: true
    },
    {
      bool: true
    },
    {
      bool: true
    },
    ],

    switch_key: [{
      key: 'switch1',
    },
    {
      key: 'switch2'
    },
    {
      key: 'switch3'
    },
    {
      key: 'switch4'
    },
    {
      key: 'switch5'
    },
    {
      key: 'switch6'
    },
    {
      key: 'switch7'
    }
    ],
    temporary_clock_sum: 0,

    index_delete: 0,

    clock: [{
      clock: wx.getStorageSync('clock1'),
    },
    {
      clock: wx.getStorageSync('clock2')
    },
    {
      clock: wx.getStorageSync('clock3')
    },
    {
      clock: wx.getStorageSync('clock4')
    },
    {
      clock: wx.getStorageSync('clock5')
    },
    {
      clock: wx.getStorageSync('clock6')
    },
    {
      clock: wx.getStorageSync('clock7')
    },
    ],

    send_object: [{
      clock: ''
    },
    {
      clock: ''
    },
    {
      clock: ''
    },
    {
      clock: ''
    },
    {
      clock: ''
    },
    {
      clock: ''
    },
    {
      clock: ''
    },
    ],
  },

  onLoad: function () {
    var clock_ = this.data.clock;
    clock_[0].clock = wx.getStorageSync('clock1'),
      clock_[1].clock = wx.getStorageSync('clock2'),
      clock_[2].clock = wx.getStorageSync('clock3'),
      clock_[3].clock = wx.getStorageSync('clock4'),
      clock_[4].clock = wx.getStorageSync('clock5'),
      clock_[5].clock = wx.getStorageSync('clock6'),
      clock_[6].clock = wx.getStorageSync('clock7');

    var temporary_switch = this.data.switch_data;
    let switch_key = this.data.switch_key;
    for (var i = 0; i < 7; i++) {
      temporary_switch[i].bool = wx.getStorageSync(switch_key[i].key);
    }

    this.setData({
      switch_data: temporary_switch,
      clock: clock_,
      temporary_clock_sum: wx.getStorageSync('clock_sum'),
    })
  },

  edit_clock: function (e) {
    let index_edit = Number(e.currentTarget.id) + 1;
    console.log("即将编辑的闹钟编号：" + index_edit);
    wx.setStorageSync('index_edit', index_edit);
    wx.redirectTo({
      url: '../clock LS/clock LS'
    }) //记录要编辑的闹钟编号
  },

  delete_clock: function (e) {
    let index_delete_ = Number(e.currentTarget.id) + 1;
    console.log("要删除的闹钟编号：" + index_delete_);
    wx.setStorageSync('index_delete', index_delete_);
    this.setData({
      index_delete: index_delete_
    }) //记录要删除的闹钟编号
    var that = this;
    wx.showModal({
      title: '提示',
      content: '是否删除该闹钟',
      success: function (res) {
        if (res.confirm) {
          var index_delete = that.data.index_delete;
          var clock_number_key = that.data.clock_number_key;
          var switch_key = that.data.switch_key;
          wx.removeStorageSync(clock_number_key[index_delete - 1].key);
          wx.removeStorageSync(switch_key[index_delete - 1].key); //把数据清掉

          let key5 = that.data.key5;
          var temporary_clock_sum = that.data.temporary_clock_sum;
          that.setData({
            temporary_clock_sum: temporary_clock_sum - 1
          });
          console.log("删除后剩余闹钟数：" + that.data.temporary_clock_sum);
          wx.setStorageSync(key5, that.data.temporary_clock_sum); //设置删除后的闹钟数

          var temporary_clock_sum = that.data.temporary_clock_sum;
          for (var i = Number(index_delete); i <= Number(temporary_clock_sum); i++) {
            var temp = wx.getStorageSync(clock_number_key[i].key);
            var temp_bool = wx.getStorageSync(switch_key[i].key);
            wx.setStorageSync(clock_number_key[i - 1].key, temp);
            wx.setStorageSync(switch_key[i - 1].key, temp_bool);
          }
          wx.removeStorageSync(clock_number_key[temporary_clock_sum].key);
          wx.setStorageSync(switch_key[temporary_clock_sum].key, true);
          //顺势移动闹钟编号

          wx.showToast({
            title: '删除成功!',
            icon: 'none'
          })
        }
      }

    })
  },
  onPullDownRefresh: function () {
    var clock_ = this.data.clock;
    clock_[0].clock = wx.getStorageSync('clock1'),
      clock_[1].clock = wx.getStorageSync('clock2'),
      clock_[2].clock = wx.getStorageSync('clock3'),
      clock_[3].clock = wx.getStorageSync('clock4'),
      clock_[4].clock = wx.getStorageSync('clock5'),
      clock_[5].clock = wx.getStorageSync('clock6'),
      clock_[6].clock = wx.getStorageSync('clock7');

    var temporary_switch = this.data.switch_data;
    let switch_key = this.data.switch_key;
    for (var i = 0; i < 7; i++) {
      temporary_switch[i].bool = wx.getStorageSync(switch_key[i].key);
    }

    this.setData({
      switch_data: temporary_switch,
      clock: clock_,
      temporary_clock_sum: wx.getStorageSync('clock_sum'),
    })

    wx.stopPullDownRefresh(); //下拉刷新删除后的闹钟信息
  },

  switch_change: function (e) {
    let index_switch = Number(e.currentTarget.id) + 1;
    let switch_key = this.data.switch_key;
    console.log("开关的闹钟编号：" + index_switch); //获取开关的闹钟编号

    var temporary_switch = this.data.switch_data;
    for (var i = 0; i < this.data.temporary_clock_sum; i++) {
      if (i == index_switch - 1) temporary_switch[i].bool = e.detail.value;
    }
    this.setData({
      switch_data: temporary_switch
    }) //根据操作更新switch

    for (var i = 0; i < this.data.temporary_clock_sum; i++) {
      wx.setStorageSync(switch_key[i].key, this.data.switch_data[i].bool);
    } //更改数据库
    console.log("目前状态为" + this.data.switch_data[index_switch - 1].bool)
  }

})